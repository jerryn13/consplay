#pragma once

#include <wchar.h>

#include "../dependency/fetch/fetch.c"
#include "../dependency/string/string.c"
#include "../dependency/json/cJson.h"
#include "../dependency/json/cJson.c"

#include "game.c"
#include "gamechoose.c"
#include "interact.c"
#include "files.c"
#include "configs.c"

// 消息存储方式：
// [system,assistant],{[user,assistant],[user,assistant],....}
// history为{}的内容
#define LLM_HISTORT_MESSAGE_NUM 32

enum LLMChatMessageRole
{
  SYSTEM,
  ASSISTANT,
  USER
};

typedef struct
{
  char role[32];
  char content[STRING_DEFAULT_LENGTH];
} LLMChatMessage;

LLMChatMessage llm_messages[LLM_HISTORT_MESSAGE_NUM];
int llm_messages_count = 0;

void LLMInitialize()
{
  memset(llm_messages, 0, sizeof(llm_messages));
  llm_messages_count = 0;
}

void LLMChatMessageAppend(enum LLMChatMessageRole role, char *content)
{
  // 若消息数量超出携带历史消息数量限制，整体往前移两条消息

  if (llm_messages_count >= LLM_HISTORT_MESSAGE_NUM)
  {
    for (size_t i = 0; i < LLM_HISTORT_MESSAGE_NUM - 2; i++)
    {
      llm_messages[i] = llm_messages[i + 2];
    }
    memset(&llm_messages[LLM_HISTORT_MESSAGE_NUM - 2], 0, 2 * sizeof(LLMChatMessage));
    llm_messages_count -= 2;
  }

  strcpy(llm_messages[llm_messages_count].role, role == ASSISTANT ? "assistant" : "user");
  strcpy(llm_messages[llm_messages_count].content, content);
  llm_messages_count++;
}

// 流式输出时更新最后一条消息的内容
void LLMChatMessageAppendToLastOneContent(const char *content)
{
  if (llm_messages_count > 0)
  {
    strncat(llm_messages[llm_messages_count - 1].content, content, STRING_DEFAULT_LENGTH - strlen(llm_messages[llm_messages_count - 1].content) - 1);
  }
}

void LLMChunkReceiveCallback(const char *sse_chunk)
{
  char clipped_chunk[STRING_DEFAULT_LENGTH] = "";
  strncpy(clipped_chunk, sse_chunk + 6, strlen(sse_chunk) - 6);
  clipped_chunk[strlen(sse_chunk) - 6 + 1] = '\0';

  if (config_debug_mode)
  {
    printf("Debug received SSE chunk: %s\n", sse_chunk);
    printf("Debug clipped chunk: %s\n", clipped_chunk);
  }

  cJSON *root = cJSON_Parse(clipped_chunk);
  if (root == NULL)
  {
    return;
  }

  // 获取choices[0].delta.content

  cJSON *choices = cJSON_GetObjectItem(root, "choices");
  if (!choices || !cJSON_IsArray(choices))
  {
    cJSON_Delete(root);
    return;
  }

  cJSON *choice = cJSON_GetArrayItem(choices, 0);
  if (!choice || !cJSON_IsObject(choice))
  {
    cJSON_Delete(root);
    return;
  }

  cJSON *delta = cJSON_GetObjectItem(choice, "delta");
  if (!delta || !cJSON_IsObject(delta))
  {
    cJSON_Delete(root);
    return;
  }

  cJSON *finish_reason = cJSON_GetObjectItem(choice, "finish_reason");
  if (finish_reason && cJSON_IsString(finish_reason))
  {
    if (strcmp(finish_reason->valuestring, "stop") == 0)
    {
      printf("\n");
      printf("————————————————————\n");
      printf("你: ");

      cJSON_Delete(root);

      return;
    }
  }

  cJSON *content = cJSON_GetObjectItem(delta, "content");
  if (!content || !cJSON_IsString(content))
  {
    cJSON_Delete(root);
    return;
  }

  const char *contentStr = content->valuestring;

  printf("%s", contentStr);
  LLMChatMessageAppendToLastOneContent(contentStr);

  cJSON_Delete(root);
}

char llm_read_buffer[2048] = "";

void LLMSSECallback(const char *buffer_chunk)
{
  // 收到的是长度为1024的buffer_chunk，而不是SSE chunk, 需要堆积buffer_chunk，自行分割出SSE chunk。
  // 每个SSE chunk的开头是"data: "

  strcat(llm_read_buffer, buffer_chunk);

  // 用来存储 SSE chunk
  char sse_chunk[STRING_DEFAULT_LENGTH] = "";
  size_t llm_len = strlen(llm_read_buffer);
  size_t start_pos = 0;

  // 遍历read_buffer，找到每个 "data: " 就割一刀
  for (size_t i = 0; i < llm_len; i++)
  {
    if (strncmp(llm_read_buffer + i, "data: ", 6) == 0)
    {
      // 如果 start_pos 已经记录了上一个 chunk，分割出来
      if (i > start_pos)
      {
        strncpy(sse_chunk, llm_read_buffer + start_pos, i - start_pos);
        sse_chunk[i - start_pos] = '\0';
        LLMChunkReceiveCallback(sse_chunk);
      }
      start_pos = i; // 记录新 SSE chunk 的起始位置
    }
  }

  // 处理剩余未完整输出的部分
  if (start_pos < llm_len)
  {
    strcpy(llm_read_buffer, llm_read_buffer + start_pos);
  }
  else
  {
    llm_read_buffer[0] = '\0';
  }
}

void LLMSubmit(char *user_input, char *init_system_prompt, char *init_assistant_message)
{
  // 插入聊天记录
  LLMChatMessageAppend(USER, user_input);

  // 构建请求体
  cJSON *json_body = cJSON_CreateObject();
  cJSON *messages_array = cJSON_CreateArray();

  cJSON *system_message = cJSON_CreateObject();
  cJSON_AddStringToObject(system_message, "role", "system");
  cJSON_AddStringToObject(system_message, "content", init_system_prompt);
  cJSON_AddItemToArray(messages_array, system_message);

  cJSON *assistant_message = cJSON_CreateObject();
  cJSON_AddStringToObject(assistant_message, "role", "assistant");
  cJSON_AddStringToObject(assistant_message, "content", init_assistant_message);
  cJSON_AddItemToArray(messages_array, assistant_message);

  for (int i = 0; i < llm_messages_count; i++)
  {
    cJSON *message = cJSON_CreateObject();
    cJSON_AddStringToObject(message, "role", llm_messages[i].role);
    cJSON_AddStringToObject(message, "content", llm_messages[i].content);
    cJSON_AddItemToArray(messages_array, message);
  }

  cJSON_AddItemToObject(json_body, "messages", messages_array);
  cJSON_AddBoolToObject(json_body, "stream", cJSON_True);
  cJSON_AddStringToObject(json_body, "model", config_llm_model);
  char *body_string = cJSON_PrintUnformatted(json_body);

  char headers_string[2048] = "Content-Type: application/json\r\nAuthorization: ";
  strcat(headers_string, config_llm_authorization);
  strcat(headers_string, "\n\r");

  wchar_t wchar_url[STRING_DEFAULT_LENGTH];
  wchar_t wchar_body_string[9192];
  wchar_t wchar_headers_string[STRING_DEFAULT_LENGTH];

  mbstowcs(wchar_url, config_llm_endpoint, STRING_DEFAULT_LENGTH);
  mbstowcs(wchar_body_string, body_string, 4096);
  mbstowcs(wchar_headers_string, headers_string, STRING_DEFAULT_LENGTH);

  if (config_debug_mode)
  {
    printf("debug body: %s\n\n", body_string);
    printf("debug headers: %s\n\n", headers_string);
  }

  fetchParams fetch_params = {
      .url = wchar_url,
      .method = L"POST",
      .body = body_string,
      .headers = wchar_headers_string,
      .timeout = 60000,
      .enableSSE = true,
      .onSSEChunkReceived = &LLMSSECallback};

  fetch(&fetch_params);

  free(body_string);
  cJSON_Delete(json_body);
}

// Copyright 2011 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "jsondump.h"

#include <sstream>
#include <stdio.h>
#include <string>

#include "graph.h"

/// Print a JSON string with the proper escaping.
static void printJsonString(const char *str) {
  while (*str) {
    if (*str == '"' || *str == '\\')
      putchar('\\');
    putchar(*str);
    str++;
  }
}

/// Print an array of JSON string.
static void printJsonStringArray(const std::vector<std::string>& v) {
  bool first = true;
  printf("[ ");
  for (std::vector<std::string>::const_iterator it = v.begin();
      it != v.end(); ++it) {
    if (!first) {
      printf(", ");
    }
    printf("\"");
    printJsonString(it->c_str());
    printf("\"");
    first = false;
  }
  printf(" ]");
}

void JsonDump::AddTarget(Node* node) {
  if (visited_nodes_.find(node) != visited_nodes_.end())
    return;

  visited_nodes_.insert(node);

  Edge* edge = node->in_edge();
  if (!edge) {
    return;
  }

  if (visited_edges_.find(edge) != visited_edges_.end())
    return;
  visited_edges_.insert(edge);

  std::vector<std::string> inputs;
  for (vector<Node*>::iterator in = edge->inputs_.begin();
      in != edge->inputs_.end(); ++in) {
    inputs.push_back((*in)->path());
  }

  std::vector<std::string> outputs;
  for (vector<Node*>::iterator out = edge->outputs_.begin();
      out != edge->outputs_.end(); ++out) {
    outputs.push_back((*out)->path());
  }

  if (!first_) {
    putchar(',');
  }
  printf("\n    {\n");
  printf("      \"inputs\": ");
  printJsonStringArray(inputs);
  printf(",\n");
  printf("      \"outputs\": ");
  printJsonStringArray(outputs);
  if (!edge->is_phony()) {
    printf(",\n");
    std::string cmd = edge->EvaluateCommand();
    printf("      \"cmd\": \"");
    printJsonString(cmd.c_str());
    putchar('"');
  }
  printf("\n    }");

  first_ = false;
  for (vector<Node*>::iterator in = edge->inputs_.begin();
      in != edge->inputs_.end(); ++in) {
    AddTarget(*in);
  }
}

void JsonDump::Start() {
  printf("{\n");
  printf("  \"rules\":\n");
  printf("    [");
}

void JsonDump::Finish() {
  printf("\n    ]\n");
  printf("}\n");
}


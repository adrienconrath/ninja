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

#ifndef NINJA_JSONDUMP_H_
#define NINJA_JSONDUMP_H_

/// This file provides a tool for dumping Ninja's internal graph in JSON
/// format. The dumped JSON contains an array of rules. Each rule contains:
/// - an array of input files;
/// - an array of output files;
/// - The command to run (if not a phony edge).

#include <set>
using namespace std;

struct Node;
struct Edge;

struct JsonDump {
  JsonDump() : first_(true) {}
  void Start();
  void AddTarget(Node* node);
  void Finish();

  set<Node*> visited_nodes_;
  set<Edge*> visited_edges_;
  bool first_;
};

#endif  // NINJA_JSONDUMP_H_

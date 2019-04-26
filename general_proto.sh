#!/bin/bash
protoc --proto_path=src/  --cpp_out=./src/generate  src/message.proto 

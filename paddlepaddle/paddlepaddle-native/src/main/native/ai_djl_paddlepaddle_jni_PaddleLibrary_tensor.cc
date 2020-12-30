/*
 * Copyright 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You may not use this file except in compliance
 * with the License. A copy of the License is located at
 *
 * http://aws.amazon.com/apache2.0/
 *
 * or in the "license" file accompanying this file. This file is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
 * OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions
 * and limitations under the License.
 */
#include "ai_djl_paddlepaddle_jni_PaddleLibrary.h"

#include <djl/utils.h>

#include <paddle_api.h>

JNIEXPORT jlong JNICALL Java_ai_djl_paddlepaddle_jni_PaddleLibrary_paddleCreateTensor
  (JNIEnv* env, jobject jthis, jobject jbuffer, jlong jlength, jintArray jshape, jint jdtype) {
  auto tensor_ptr = new paddle::PaddleTensor{};
  tensor_ptr->data.Reset(env->GetDirectBufferAddress(jbuffer), jlength);
  tensor_ptr->dtype = static_cast<paddle::PaddleDType>(jdtype);
  tensor_ptr->shape = djl::utils::jni::GetVecFromJIntArray(env, jshape);
  return reinterpret_cast<uintptr_t>(tensor_ptr);
}

JNIEXPORT void JNICALL Java_ai_djl_paddlepaddle_jni_PaddleLibrary_deleteTensor
  (JNIEnv* env, jobject jthis, jlong jhandle) {
  const auto* tensor_ptr = reinterpret_cast<paddle::PaddleTensor*>(jhandle);
  delete tensor_ptr;
}

JNIEXPORT jintArray JNICALL Java_ai_djl_paddlepaddle_jni_PaddleLibrary_getTensorShape
  (JNIEnv* env, jobject jthis, jlong jhandle) {
  auto tensor_ptr = reinterpret_cast<paddle::PaddleTensor*>(jhandle);
  auto shape = tensor_ptr->shape;
  int len = shape.size();
  jintArray jarray = env->NewIntArray(len);
  env->SetIntArrayRegion(jarray, 0, len, reinterpret_cast<jint*>(shape.data()));
  return jarray;
}

JNIEXPORT jint JNICALL Java_ai_djl_paddlepaddle_jni_PaddleLibrary_getTensorDType
  (JNIEnv* env, jobject jthis, jlong jhandle) {
  auto tensor_ptr = reinterpret_cast<paddle::PaddleTensor*>(jhandle);
  return tensor_ptr->dtype;
}

JNIEXPORT jobject JNICALL Java_ai_djl_paddlepaddle_jni_PaddleLibrary_getTensorData
  (JNIEnv* env, jobject jthis, jlong jhandle) {
  auto tensor_ptr = reinterpret_cast<paddle::PaddleTensor*>(jhandle);
  auto buf = &tensor_ptr->data;
  int len = buf->length();
  return env->NewDirectByteBuffer(buf->data(), len);
}
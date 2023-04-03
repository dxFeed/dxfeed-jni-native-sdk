#pragma once

#include <jni.h>
#include <string>
#include <vector>

#include "DxFeed.h"
#include "EventTypes.h"
#include "utils/Diagnostic.h"
#include "utils/TimeAndSaleFormatter.h"

namespace dxfeed {
  struct Subscription final {
    typedef void(Listener)(const void* events, std::size_t count);

    Subscription(JNIEnv* env, jobject connection, EventType eventType, dxfeed::OnCloseHandler onClose);
    Subscription(const Subscription& other) = delete;
    Subscription(Subscription&& other) = delete;
    Subscription& operator=(const Subscription& other) = delete;
    Subscription& operator=(Subscription&& other) = delete;
    ~Subscription();

    void addListener(Listener listener) const;
    void addSymbol(const std::string& symbol) const;
//    void addSymbols(const std::vector <std::string>& symbols) const;
    jobject getSub();


    // diagnostic
    void addDiagnosticListener(int64_t listener) const;
  private:
    jobject subscription_;
    JNIEnv* env_;
    const dxfeed::OnCloseHandler onClose_;
  };
}
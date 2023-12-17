// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

package com.dxfeed.api;

import com.dxfeed.api.buffers.CString;

public class DxThreadException {
  private static byte[] getExceptionInfo(Throwable nThrowable) {
    if (nThrowable != null) {
      StringBuilder sb = new StringBuilder();
      StackTraceElement[] stackTraceElements = nThrowable.getStackTrace();
      for (StackTraceElement traceElement : stackTraceElements) {
        sb.append(traceElement).append("\n\t");
      }
//      nThrowable.printStackTrace();
      String stackTrace = sb.toString();
      String message = nThrowable.getMessage();
      String className = nThrowable.getClass().getName();
      byte[] classNameBytes = CString.toCString(className);
      byte[] messageBytes = CString.toCString(message);
      byte[] stackTraceBytes = CString.toCString(stackTrace);

      int pos = 0;
      byte[] result = new byte[classNameBytes.length + messageBytes.length + stackTraceBytes.length];
      System.arraycopy(classNameBytes, 0, result, pos, classNameBytes.length);
      pos += classNameBytes.length;
      System.arraycopy(messageBytes, 0, result, pos, messageBytes.length);
      pos += messageBytes.length;
      System.arraycopy(stackTraceBytes, 0, result, pos, stackTraceBytes.length);

//      Throwable cause = nThrowable.getCause();
//      byte[] causeBytes = (cause != null) ? getExceptionInfo(cause) : null;
//      if (causeBytes != null) {
//        int oldLength = result.length;
//        result = Arrays.copyOf(result, oldLength + causeBytes.length);
//        System.arraycopy(causeBytes, 0, result, oldLength, causeBytes.length);
//      }

      return result;
    }
    return null;
  }

  // todo: for tests
  private static void exceptionSample() {
    exceptionSampleFrame1();
  }

  private static void exceptionSampleFrame1() {
    exceptionSampleFrame2();
  }

  private static void exceptionSampleFrame2() {
    throw new RuntimeException("Exception occurred in exceptionSampleFrame2");
  }
}

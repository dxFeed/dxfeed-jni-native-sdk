// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

package com.dxfeed.api;

// https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/java/com/dxfeed/sdk/symbol/DxfgSymbolType.java#L10
public enum DxfgSymbolType {
  STRING,
  CANDLE,
  WILDCARD,
  INDEXED_EVENT_SUBSCRIPTION,
  TIME_SERIES_SUBSCRIPTION
}

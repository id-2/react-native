/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.react.views.modal;

import androidx.annotation.Nullable;
import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.common.ViewUtil;
import com.facebook.react.uimanager.events.Event;

/** {@link Event} for dismissing a Dialog. */
/* package */ class DismissEvent extends Event<DismissEvent> {

  public static final String EVENT_NAME = "topDismiss";

  @Deprecated
  protected DismissEvent(int viewTag) {
    this(ViewUtil.NO_SURFACE_ID, viewTag);
  }

  protected DismissEvent(int surfaceId, int viewTag) {
    super(surfaceId, viewTag);
  }

  @Override
  public String getEventName() {
    return EVENT_NAME;
  }

  @Nullable
  @Override
  protected WritableMap getEventData() {
    return Arguments.createMap();
  }
}

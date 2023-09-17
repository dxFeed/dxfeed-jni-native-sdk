package com.dxfeed.api.utils;

import com.fasterxml.jackson.annotation.JsonAutoDetect;
import com.fasterxml.jackson.annotation.PropertyAccessor;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;

import java.util.logging.Level;
import java.util.logging.Logger;

public class XmlMapping {
  private static final Logger logger = Logger.getLogger(XmlMapping.class.getCanonicalName());
  private static final ObjectMapper OBJECT_MAPPER = new ObjectMapper();

  static {
    OBJECT_MAPPER.setVisibility(PropertyAccessor.ALL, JsonAutoDetect.Visibility.NONE);
    OBJECT_MAPPER.setVisibility(PropertyAccessor.FIELD, JsonAutoDetect.Visibility.ANY);
    OBJECT_MAPPER.setVisibility(PropertyAccessor.CREATOR, JsonAutoDetect.Visibility.ANY);
  }

  public static String toString(Object obj) {
    if (obj != null) {
      try {
        return OBJECT_MAPPER.writeValueAsString(obj);
      } catch (final JsonProcessingException e) {
        logger.log(Level.WARNING, e.getMessage(), e);
      }
    }
    return "";
  }

  public static Object fromString(String data, Class<?> attachmentClass) {
    try {
      OBJECT_MAPPER.readValue(data, attachmentClass);
    } catch (final JsonProcessingException e) {
      logger.log(Level.WARNING, e.getMessage(), e);
    }
    return null;
  }
}

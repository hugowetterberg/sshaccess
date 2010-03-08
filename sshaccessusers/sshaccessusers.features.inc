<?php

/**
 * Implementation of hook_content_default_fields().
 */
function sshaccessusers_content_default_fields() {
  module_load_include('inc', 'sshaccessusers', 'sshaccessusers.defaults');
  $args = func_get_args();
  return call_user_func_array('_sshaccessusers_content_default_fields', $args);
}

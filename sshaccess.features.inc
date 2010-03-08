<?php

/**
 * Implementation of hook_content_default_fields().
 */
function sshaccess_content_default_fields() {
  module_load_include('inc', 'sshaccess', 'sshaccess.defaults');
  $args = func_get_args();
  return call_user_func_array('_sshaccess_content_default_fields', $args);
}

/**
 * Implementation of hook_node_info().
 */
function sshaccess_node_info() {
  module_load_include('inc', 'sshaccess', 'sshaccess.features.node');
  $args = func_get_args();
  return call_user_func_array('_sshaccess_node_info', $args);
}

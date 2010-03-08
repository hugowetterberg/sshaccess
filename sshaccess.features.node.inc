<?php

/**
 * Helper to implementation of hook_node_info().
 */
function _sshaccess_node_info() {
  $items = array(
    'remote_account' => array(
      'name' => t('Remote Account'),
      'module' => 'features',
      'description' => t('Describes a remote server account that Drupal should manage SSH access for.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'has_body' => '1',
      'body_label' => t('Description'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

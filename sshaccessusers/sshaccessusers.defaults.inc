<?php

/**
 * Helper to implementation of hook_content_default_fields().
 */
function _sshaccessusers_content_default_fields() {
  $fields = array();

  // Exported field: field_access_users
  $fields[] = array(
    'field_name' => 'field_access_users',
    'type_name' => 'remote_account',
    'display_settings' => array(
      'label' => array(
        'format' => 'above',
        'exclude' => 0,
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'userreference',
    'required' => '0',
    'multiple' => '1',
    'module' => 'userreference',
    'active' => '1',
    'referenceable_roles' => array(
      '2' => 0,
    ),
    'referenceable_status' => '',
    'widget' => array(
      'autocomplete_match' => 'contains',
      'size' => '30',
      'reverse_link' => 0,
      'default_value' => array(
        '0' => array(
          'uid' => NULL,
          '_error_element' => 'default_value_widget][field_access_users][0][uid][uid',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Users',
      'weight' => '-2',
      'description' => '',
      'type' => 'userreference_autocomplete',
      'module' => 'userreference',
    ),
  );

  // Translatables
  array(
    t('Users'),
  );

  return $fields;
}

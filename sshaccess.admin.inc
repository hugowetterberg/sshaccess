<?php

function sshaccess_settings_form() {
  $form = array();

  $form['sshaccess_ssh_keygen_path'] = array(
    '#type' => 'textfield',
    '#title' => t('SSH keygen executable'),
    '#description' => t('Enter the full path to the ssh_keygen executable.'),
    '#size' => 40,
    '#maxlength' => 255,
    '#default_value' => variable_get('sshaccess_ssh_keygen_path', ''),
    '#required' => TRUE,
  );

  $form['sshaccess_job_directory'] = array(
    '#type' => 'textfield',
    '#title' => t('Job directory'),
    '#description' => t('The path to the directory where jobs and private keys will be placed. This directory must <strong>not</strong> be exposed to external access.'),
    '#size' => 30,
    '#maxlength' => 255,
    '#default_value' => variable_get('sshaccess_job_directory', ''),
    '#required' => TRUE,
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

function sshaccess_settings_form_validate($form, $form_state) {
  $values = $form_state['values'];

  // Validate the ssh-keygen path.
  if (!file_exists($values['sshaccess_ssh_keygen_path'])) {
    form_set_error('sshaccess_ssh_keygen_path', t('The ssh-keygen executable doesn\'t exist'));
  }
  else if (!is_executable($values['sshaccess_ssh_keygen_path'])) {
    form_set_error('sshaccess_ssh_keygen_path', t('The ssh-keygen executable isn\'t executable'));
  }

  // Validate and, if necessary, create the job directory.
  if (file_exists($values['sshaccess_job_directory'])) {
    if (!is_writable($values['sshaccess_job_directory'])) {
      form_set_error('sshaccess_job_directory', t('The job directory is not writeable'));
    }
  }
  else {
    if (!mkdir($values['sshaccess_job_directory'])) {
      form_set_error('sshaccess_job_directory', t('The job directory doesn\'t exist and cannot be created automatically.'));
    }
    else {
      drupal_set_message(t('The directory @dir has been created', array(
        '@dir' => $values['sshaccess_job_directory'],
      )));
    }
  }
}

function sshaccess_settings_form_submit($form, $form_state) {
  $values = $form_state['values'];

  variable_set('sshaccess_ssh_keygen_path', $values['sshaccess_ssh_keygen_path']);
  variable_set('sshaccess_job_directory', $values['sshaccess_job_directory']);
  drupal_goto();
}
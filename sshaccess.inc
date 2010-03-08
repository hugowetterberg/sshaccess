<?php

/**
 * Generates master keypair for a remote account and returns the public key.
 *
 * @param int $nid
 * @return string
 *  The public key.
 */
function sshaccess_generate_keypair($nid) {
  module_load_include('inc', 'sshkey');
  $job_dir = variable_get('sshaccess_job_directory', '');
  $ssh_keygen = variable_get('sshaccess_ssh_keygen_path', '');

  $retval = 0;
  $output = array();
  exec(sprintf("%s -f %s/%d -N '' -b 2048", $ssh_keygen, $job_dir, $nid), $output, $retval);

  $pubkey = FALSE;
  if ($retval == 0) {
    $pubkey = file_get_contents(sprintf('%s/%d.pub', $job_dir, $nid));
  }
  return sshkey_public_key_parse($pubkey);
}

/**
 * Updates the authorized_keys file for the remote account.
 *
 * @param object $node
 * @param array $data
 *  The data to pass to the job. Should at least contain 'host' and 'user'.
 * @return void
 */
function sshaccess_update_authorized_keys($node, $data=array()) {
  $res = db_query("SELECT fingerprint FROM sshaccess
    WHERE nid = %d", array(
      ':nid' => $node->nid,
  ));

  // Generate a list of public keys to pass to the 
  $pubkeys = module_invoke_all('sshaccess_authorized_keys', $node);
  while ($fingerprint = db_result($res)) {
    $pubkey = sshkey_get_public_key_data($fingerprint);
    if ($pubkey) {
      $pubkeys[$pubkey['fingerprint']] = $pubkey['pubkey'];
    }
  }
  drupal_alter('sshaccess_authorized_keys', $pubkeys, $node);
  error_log(var_export($pubkeys, TRUE));
  $data['pubkeys'] = $pubkeys;

  sshaccess_add_job($node->nid, 'update_authorized_keys', $data);
}

/**
 * Adds a job to the queue.
 *
 * @param int $nid
 *  The nid of the remote site.
 * @param string $job
 *  The job name.
 * @param array $data
 *  The data to pass with the job.
 * @return void
 */
function sshaccess_add_job($nid, $job, $data=array()) {
  $job_dir = variable_get('sshaccess_job_directory', '');
  $job_path = sprintf('%s/%d-%s.job', $job_dir, $nid, $job);
  file_put_contents($job_path, json_encode(array(
    'nid' => $nid,
    'name' => $job,
    'data' => $data,
  )));
}
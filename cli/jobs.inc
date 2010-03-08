<?php

function job_add_keys($job, $job_dir, $key_store) {
  $from = sprintf('%s/%d', $job_dir, $job['nid']);
  $to = sprintf('%s/%d', $key_store, $job['nid']);
  $info = sprintf('%s/%d.info', $key_store, $job['nid']);
  $success = FALSE;

  if (file_exists($from)) {
    $success = rename($from, $to);
    $success = $success && rename("{$from}.pub", "{$to}.pub");
    if ($success) {
      file_put_contents($info, sprintf("User: %s\nHost: %s\n", $job['data']['user'], $job['data']['host']));
    }
  }

  return $success;
}

function job_update_authorized_keys($job, $job_dir, $key_store) {
  // Write the keys to the authorized_keys file.
  $authorized_file = sprintf("%s/%d.authorized_keys", $key_store, $job['nid']);
  $fp = fopen($authorized_file, 'w');
  foreach ($job['data']['pubkeys'] as $pubkey) {
    fwrite($fp, $pubkey);
    fwrite($fp, "\n");
  }
  fclose($fp);

  // Keep the info file up to date.
  $info = sprintf('%s/%d.info', $key_store, $job['nid']);
  file_put_contents($info, sprintf("User: %s\nHost: %s\n", $job['data']['user'], $job['data']['host']));

  // Scp (Secure copy) the authorized keys to the server.
  $target = sprintf('%s@%s:~/.ssh/authorized_keys', $job['data']['user'], $job['data']['host']);
  $identity = sprintf("%s/%d", $key_store, $job['nid']);
  $cmd = sprintf('scp -i %s -o StrictHostKeyChecking=no %s %s', 
    escapeshellarg($identity),
    escapeshellarg($authorized_file),
    escapeshellarg($target));
  $output = array();
  $retval = 0;
  $success = exec($cmd, $output, $retval);

  unlink($authorized_file);

  return $retval == 0;
}

function job_archive_keys($job, $job_dir, $key_store) {
  $archive_dir = sprintf('%s/archived_keys', $key_store);
  if (!file_exists($archive_dir)) {
    mkdir($archive_dir);
  }

  $private = sprintf('%s/%d', $key_store, $job['nid']);
  $public = sprintf('%s/%d.pub', $key_store, $job['nid']);
  $info = sprintf('%s/%d.info', $key_store, $job['nid']);
  $zip_file = sprintf('%s/%d.zip', $key_store, $job['nid']);

  // Create the archive file.
  $zip = new ZipArchive();
  $zip->open($zip_file, ZipArchive::CREATE);
  $zip->addFile(basename($private), $private);
  $zip->addFile(basename($public), $public);
  $zip->addFile(basename($info), $info);
  $zip->close();

  // Append archive hash to the info file
  $fp = fopen($info, 'a');
  fwrite($fp, sprintf("SHA1: %s\n", hash_file('sha1', $zip_file)));
  fclose($fp);

  // Removed the archived keys and move the info file to the archive dir.
  unlink($private);
  unlink($public);
  rename($info, sprintf('%s/%d.info', $archive_dir, $job['nid']));
}
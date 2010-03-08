<?php
if ($argc !== 3) {
  print "Usage: \n\tphp do_jobs.php path/to/job_directory path/to/key_store_directory\n";
  exit(1);
}

$job_dir = realpath($argv[1]);
$key_store = realpath($argv[2]);

if (empty($job_dir)) {
  print "Missing job directory.\n";
  exit(1);
}

if (empty($key_store)) {
  print "Missing key store directory.\n";
  exit(1);
}

$jobs = array();
// Enumerate jobs, we're constructing an array where the filename is prefixed
// with the timestamp for the job file so that we'll always execute them in
// sequence. Executing a update_authorized_keys before add_keys would for
// example fail miserably.
$dh = opendir($job_dir);
while ($filename = readdir($dh)) {
  if (substr($filename, -4) == '.job') {
    $file = $job_dir . '/' . $filename;
    $key = sprintf('%d-%s', filemtime($file), $filename);
    $contents = file_get_contents($file);
    $jobs[$key] = json_decode($contents, TRUE);
    $jobs[$key]['file'] = $file;
  }
}

if (!empty($jobs)) {
  require('jobs.inc');
  
  // Get the correct execution order
  ksort($jobs);

  foreach($jobs as $job) {
    $success = FALSE;
    switch ($job['name']) {
      case 'add_keys':
        $success = job_add_keys($job, $job_dir, $key_store);
        break;
      case 'update_authorized_keys':
        $success = job_update_authorized_keys($job, $job_dir, $key_store);
        break;
      case 'archive_keys':
        $success = job_archive_keys($job, $job_dir, $key_store);
        break;
    }
    if ($success) {
      unlink($job['file']);
    }
  }
}
<div class="sshaccess-remote-account-public-key">
  <p class="message"><?php print t('This public key must be listed in the authorized_keys file for the remote account.'); ?></p>
  <textarea class="public-key" cols="50" rows="10"><?php print check_plain($pubkey['pubkey']); ?></textarea>
</div>
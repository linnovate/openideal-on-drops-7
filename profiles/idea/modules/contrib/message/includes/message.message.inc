<?php

/**
 * @file
 * A class used for messages.
 */


class Message extends Entity {

  /**
   * The message type category of the message.
   *
   * @var string
   */
  public $type;

  /**
   * The message timestamp.
   *
   * @var string
   */
  public $timestamp;

  /**
   * The message arguments.
   *
   * @see MessageType::arguments()
   *
   * @var array
   */
  public $arguments = array();

  /**
   * The message data array.
   *
   * @see MessageType::data()
   *
   * @var array
   */
  public $data = array();

  public function __construct($values = array()) {
    if (!isset($values['uid']) && isset($values['user'])) {
      $values['uid'] = $values['user']->uid;
      unset($values['user']);
    }
    if (isset($values['type']) && is_object($values['type'])) {
      $values['type'] = $values['type']->name;
    }

    parent::__construct($values, 'message');
    if (!isset($this->uid)) {
      $this->uid = $GLOBALS['user']->uid;
    }
    if (!isset($this->timestamp)) {
      $this->timestamp = time();
    }
  }

  /**
   * Returns the user associated with the message.
   */
  public function user() {
    return user_load($this->uid);
  }

  /**
   * Sets a new user associated with the message.
   *
   * @param $account
   *   The user account object or the user account id (uid).
   */
  public function setUser($account) {
    $this->uid = is_object($account) ? $account->uid : $account;
  }

  /**
   * Gets the associated message type.
   *
   * @return MessageType
   */
  public function getType() {
    return message_type_load($this->type);
  }

  /**
   * Generate an array for rendering the entity's content.
   *
   * @see entity_build_content()
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {
    $content['text'] = array(
      '#markup' => $this->getText($langcode),
    );
    return entity_get_controller($this->entityType)->buildContent($this, $view_mode, $langcode, $content);
  }

  /**
   * Replace arguments with their placeholders.
   *
   * @param $langcode
   *   Optional; The language to get the text in. If not set the current language
   *   will be used.
   * @param $options
   *   Optional; Array to be passed to MessageType::getText().
   */
  public function getText($langcode = LANGUAGE_NONE, $options = array()) {
    $message_type = $this->getType();

    if (!$message_type) {
      // Message type does not exist any more.
      return '';
    }

    $arguments = message_get_property_values($this, 'arguments');
    $output = $message_type->getText($langcode, $options);

    if (!empty($arguments)) {
      $args = array();
      foreach ($arguments as $key => $value) {
        if (is_array($value) && !empty($value['callback']) && function_exists($value['callback'])) {
          // A replacement via callback function.
          $value += array('pass message' => FALSE);
          if ($value['pass message']) {
            // Pass the message object as-well.
            $value['callback arguments'][] = $this;
          }

          $value = call_user_func_array($value['callback'], $value['callback arguments']);
        }

        switch ($key[0]) {
          case '@':
            // Escaped only.
            $args[$key] = check_plain($value);
            break;

          case '%':
          default:
            // Escaped and placeholder.
            $args[$key] = drupal_placeholder($value);
            break;

          case '!':
            // Pass-through.
            $args[$key] = $value;
        }
      }
      $output = strtr($output, $args);
    }
    $token_replace = message_get_property_values($this, 'data', 'token replace', TRUE);
    if ($output && $token_replace) {
      // Message isn't explicetly denying token replace, so process the text.
      $context = array('message' => $this);

      $token_options = message_get_property_values($this, 'data', 'token options');
      $output = token_replace($output, $context, $token_options);
    }
    return $output;
  }

  /**
   * Implements Entity::save().
   *
   * Auto create arguments based on syntax.
   *
   * If in one of the message text fields there is a token in the
   * following syntax @{message:user:name}, message will check if it is a
   * valid token, and if so, will create a new arguments, e.g.:
   *
   * @code
   *   $message->arguments['@{message:user:name}'] = 'foo';
   * @endcode
   *
   * Like this, it is possible to hardcode a token value, upon the message
   * creation, without the need to retrieve the token value each time the
   * message is displayed.
   *
   * This can be used for example, to hardcode the user's name, assuming
   * it will not change on the site.
   */
  public function save() {
    if (empty($this->is_new) || !empty($this->data['skip token hardcode'])) {
      // Message isn't new, or message explicetly doesn't want token
      // hardcoding
      parent::save();
      return;
    }

    $context = array('message' => $this);
    $token_options = !empty($this->data['token options']) ? $this->data['token options'] : array();

    $message_type = $this->getType();
    $wrapper = entity_metadata_wrapper('message_type', $message_type);

    // Iterate over the text fields.
    $tokens = array();
    foreach (field_info_instances('message_type', $message_type->category) as $instance) {
      $field_name = $instance['field_name'];
      $field = field_info_field($field_name);
      if (!in_array($field['type'], array('text', 'text_long'))) {
        // Not a text field.
        continue;
      }

      if (!$output = $message_type->getText(NULL, array('field name' => $field_name))) {
        // Field is empty.
        continue;
      }

      // Check for our hardcode syntax.
      $matches = array();
      preg_match_all('/[@|%|\!]\{([a-z0-9:_\-]+?)\}/i', $output, $matches);
      if (!$matches) {
        continue;
      }
      foreach ($matches[1] as $delta => $token) {
        $output = token_replace('[' . $token . ']', $context, $token_options);
        if ($output != '[' . $token . ']') {
          // Token was replaced.
          $argument = $matches[0][$delta];
          $tokens[$argument] = $output;
        }
      }
    }

    $this->arguments = array_merge($this->arguments, $tokens);
    parent::save();
  }
}

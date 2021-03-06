<?php
/**
 * @file
 * fe_idea.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function fe_idea_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu:ideas
  $menu_links['main-menu:ideas'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'ideas',
    'router_path' => 'ideas',
    'link_title' => 'Ideas',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-48',
  );
  // Exported menu link: user-menu:user/login
  $menu_links['user-menu:user/login'] = array(
    'menu_name' => 'user-menu',
    'link_path' => 'user/login',
    'router_path' => 'user/login',
    'link_title' => 'Login',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-50',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Ideas');
  t('Login');


  return $menu_links;
}

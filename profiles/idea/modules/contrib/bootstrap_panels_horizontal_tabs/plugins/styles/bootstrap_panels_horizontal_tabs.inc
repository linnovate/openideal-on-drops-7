<?php

$plugin = array(
  'title' => t('Horizontal Tabs implemented with Bootstrap 3.'),
  'description' => t('The title of every pane in this region will become a horizontal tab. Uses Bootstrap 3'),
  'render region' => 'bootstrap_panels_horizontal_tabs_style_render_region',
  'settings form' => 'bootstrap_panels_horizontal_tabs_style_settings_form',
  'weight' => -50,
);

/**
 * Render callback
 */
function theme_bootstrap_panels_horizontal_tabs_style_render_region($vars) {
  // Include some basic information.
  $display = $vars['display'];
  $panes = $vars['panes'];
  $did = $vars['display']->did;

  // Create the output.

  // Start the Region Wrapper.
  $output = '<div class="bootstrap-panels-horizontal-tabs">';

  // Start the Tabs.
  $output .= '<ul class="nav nav-pills">';

  $i = 0;
  foreach ($panes as $pane_id => $pane) {
    if ($i == 0 ) {
      $output .= '<li class="active">';
      $i++;
    }
    else {
      $output .= '<li>';
      $i++;
    }
    $pane_title = drupal_static($pane_id . '_title');
    if (empty($pane_title)) {
      $pane_title = 'This pane needs a title.';
    }
    $output .= '<a href="#pane-' . $pane_id . '" data-toggle="tab">' . $pane_title .  '</a>';

    $output .= '</li>';
  }
  $output .= '</ul>';
  // End the Tabs.

  // Start Panes output.
  $output .= '<div class="tab-content">';
  $i = 0;
  foreach ($panes as $pane_id => $pane) {
    $output .= '<div id="pane-' . $pane_id . '" class="tab-pane active">' . $pane . '</div>';
  }
  // End Panes output.
  $output .= '</div>';

  // End Region Wrapper.
  $output .= '</div>';

  return $output;
}

/**
 * Settings form
 */
function bootstrap_panels_horizontal_tabs_style_settings_form($style_settings) {
  $form['help_text'] = array(
    '#markup' => t('The title of each pane will be become a tab which will toggle display of that pane.
        Ensure that you give every pane a title, otherwise you will see a message reminding you to do so.'),
  );

  return $form;
}

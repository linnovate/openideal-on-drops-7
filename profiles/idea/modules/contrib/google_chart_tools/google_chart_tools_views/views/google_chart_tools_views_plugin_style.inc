<?php
/**
 * @file
 * Contains the Google Chart Tools display plugin.
 */

/**
 * The views style plugin.
 *
 * @ingroup views_style_plugins
 */
class google_chart_tools_views_plugin_style extends views_plugin_style {
  
  function option_definition () {
    $options = parent::option_definition();
    
    $options['type'] = array('default' => 'LineChart');
    $options['width'] = array('default' => 600);
    $options['height'] = array('default' => 400);  
    $options['curve'] = array('default' => 0);  
    $options['3d'] = array('default' => 0);  
    
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['grouping'] = array();
    
    $form['type'] = array(
      '#type' => 'textfield',
      '#title' => t('Type'),
      '#description' => t('Chart type, see <a href="https://google-developers.appspot.com/chart/interactive/docs/gallery">Google Chart Tools gallery</a>.'),
      '#size' => 16,
      '#required' => TRUE,
      '#description' => t('Ex. LineChart, PieChart, ColumnChart, AreaChart, Gauge, BarChart, etc....'),  
      '#default_value' => $this->options['type'],
    );
    $form['width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#description' => t('Chart width in pixels'),
      '#size' => 8,
      '#required' => TRUE,
      '#default_value' => $this->options['width'],
    );
    $form['height'] = array(
      '#type' => 'textfield',
      '#title' => t('Height'),
      '#description' => t('Chart height in pixels'),
      '#size' => 8,
      '#required' => TRUE,
      '#default_value' => $this->options['height'],
    );   
    $form['curve'] = array(
      '#type' => 'checkbox',
      '#title' => t('Curve'),
      '#description' => t('Use a curve function'),
      '#default_value' => $this->options['curve'],
    );   
    $form['3d'] = array(
      '#type' => 'checkbox',
      '#title' => t('3D'),
      '#description' => t('Make chart 3D'),
      '#default_value' => $this->options['3d'],
    );
    $form['colors'] = array(
      '#type' => 'textfield',
      '#title' => t('Colors'),
      '#description' => t('A color strings separated by commas. Ex. red, #004411'),
      '#size' => 32,
      '#default_value' => $this->options['colors'],
    );   
  }

  function render() {
    $header = array();
    $item = array();

    foreach ($this->view->field as $key => $field) {
      if ($field->position !== 0) {
        $column[] = $field->definition['title'];
      }   
    }

    foreach ($this->view->result as $row_index => $row) {
      foreach ($this->view->field as $key => $field) {
        if (!$field->options['exclude']) {
          if ($field->position === 0) {
            $header[$row_index] = $row->{$field->field_alias};
          }
          else {
            $item[$row_index][] = $row->{$field->field_alias};
          }
        }
      }
    } 
    
    $item = _flip($item);
    $settings['chart'][drupal_clean_css_identifier($this->view->name . '-' . $this->display->id)] = array(  
      'header' => $header,
      'rows' => $item,
      'columns' => $column,
      'chartType' => $this->options['type'],
      'options' => array(
        'forceIFrame' => FALSE,
        'curveType' => $this->options['curve'] ? "function" : "none",
        'is3D' => $this->options['3d'],
        'colors' => $this->options['colors'] ? explode(",", str_replace(' ', '', $this->options['colors'])) : NULL,  
        'title' => $this->view->human_name,
        'width' => $this->options['width'],
        'height' => $this->options['height']
      )   
    );

    if (strpos($_GET['q'], 'admin/structure/views/nojs/preview') === FALSE) {
       //Draw it.
      $ret = draw_chart($settings); 
      return $ret['markup'];
    }
  }
}

function _flip($arr) {
  $out = array();
  foreach ($arr as $key => $subarr) {
    foreach ($subarr as $subkey => $subvalue) {
      $out[$subkey][$key] = $subvalue;
    }
  }
  return $out;
}
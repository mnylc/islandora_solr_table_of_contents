<?php

/**
 * @file
 * Block functionality for table of contents.
 */

/**
 * Rendering handler for the table of contents.
 *
 * @param AbstractObject $object
 *   The object to render for.
 *
 * @return array
 *   The table of contents.
 */
function islandora_solr_table_of_contents_table_of_contents($object) {
  global $base_url;
  $module_path = drupal_get_path('module', 'islandora_solr_table_of_contents');
  $library_path = libraries_get_path('jstree');
  // It's a hard dependency.
  if (!$library_path) {
    watchdog('islandora_solr_table_of_contents', "jsTree library is not installed.");
    return;
  }
  $id = drupal_html_id('islandora_solr_table_of_contents_tree');
  // Make a tree!
  $form['tree'] = array(
    '#type' => 'markup',
    '#markup' => "<div id='$id'></div>",
    '#attached' => array(
      'css' => array(
        "$library_path/dist/themes/default/style.min.css",
        "$module_path/css/table_of_contents.css",
      ),
      'js' => array(
        "$library_path/dist/libs/jquery.js" => array(
          'group' => JS_LIBRARY,
        ),
        "$library_path/dist/jstree.min.js" => array(
          'group' => JS_LIBRARY,
        ),
        "$library_path/src/misc.js" => array(
          'group' => JS_LIBRARY,
        ),
        "$module_path/js/table_of_contents.js" => array(
          'group' => JS_LIBRARY,
        ),
      ),
    ),
  );

  // Finally, attach our jsTree settings.
  $form['tree']['#attached']['js'][] = array(
    'type' => 'setting',
    'data' => array(
      'islandora_solr_table_of_contents' => array(
        'jstree' => array(
          'info' => array(
            $id => array(
              'pid' => $object->id,
              'plugins' => variable_get('islandora_solr_table_of_contents_save_state', TRUE) ? array('state') : array(),
            ),
          ),
        ),
      ),
    ),
  );
  return $form;
}

/**
 * Configuration form for the table of contents block.
 *
 * @return array
 *   Form definition for the table of contents.
 */
function islandora_solr_table_of_contents_configuration_form() {
  if (user_access(ISLANDORA_SOLR_TABLE_OF_CONTENTS_MANAGE_TABLE_OF_CONTENTS)) {
    module_load_include('inc', 'islandora', 'includes/utilities');
    $header = array(
      'selected' => t('Enabled'),
      'cm_label' => t('Content Model'),
      'solr_field' => t('Child Relationship Solr Field'),
      'sort_field' => t('Sort Field (Optional)'),
    );
    $options = array();
    $content_models = islandora_get_content_models();
    $defaults = variable_get('islandora_solr_table_of_contents_content_models', islandora_solr_table_of_contents_get_default_configuration());
    foreach ($content_models as $content_model) {
      $label = $content_model['label'];
      $pid = $content_model['pid'];
      $options[$pid] = array(
        'selected' => array(
          '#type' => 'checkbox',
          '#default_value' => isset($defaults[$pid]),
        ),
        'cm_label' => array(
          '#markup' => t('@label (@pid)', array('@label' => $label, '@pid' => $pid)),
        ),
        'solr_field' => array(
          '#type' => 'textfield',
          '#default_value' => isset($defaults[$pid]) && isset($defaults[$pid]['solr_field']) ? $defaults[$pid]['solr_field'] : NULL,
        ),
        'sort_field' => array(
          '#type' => 'textfield',
          '#default_value' => isset($defaults[$pid]) && isset($defaults[$pid]['sort_field']) ? $defaults[$pid]['sort_field'] : NULL,
        ),
      );
    }
    return array(
      'has_model_field' => array(
        '#type' => 'textfield',
        '#title' => t('hasModel Solr Field'),
        '#default_value' => variable_get('islandora_solr_table_of_contents_has_model_field', ISLANDORA_SOLR_TABLE_OF_CONTENTS_HAS_MODEL_DEFAULT),
        '#description' => t('Enter the Solr field to use to get the hasModel relationship of an object.'),
        '#element_validate' => array('islandora_solr_table_of_contents_validate_string'),
      ),
      'label_field' => array(
        '#type' => 'textfield',
        '#title' => t('Label Solr Field'),
        '#default_value' => variable_get('islandora_solr_table_of_contents_label_field', ISLANDORA_SOLR_TABLE_OF_CONTENTS_LABEL_DEFAULT),
        '#description' => t('Enter the Solr field to use as labels for items in the table of contents.'),
        '#element_validate' => array('islandora_solr_table_of_contents_validate_string'),
      ),
      'save_state' => array(
        '#type' => 'checkbox',
        '#title' => t('Save Table of Contents State?'),
        '#default_value' => variable_get('islandora_solr_table_of_contents_save_state', 1),
        '#description' => t('Check this to save which nodes are open between pages. Note that for repositories with larger, deeper hierarchies this can require several calls to Solr and may be undesirable. The unchecked behaviour is to only load hierarchies from the current object, unexpanded.'),
      ),
      'root_node' => array(
        '#type' => 'radios',
        '#title' => t('Start from:'),
        '#default_value' => variable_get('islandora_solr_table_of_contents_root_node', 'last_collection'),
        '#description' => t('Select which related node the table of contents should attempt to start from.'),
        '#options' => array(
          'root_collection' => t('The configured root collection'),
          'last_collection' => t('The current or last-visited collection'),
          'current_object' => t('The current object'),
        ),
        '#states' => array(
          'invisible' => array(
            ':input[name="save_state"]' => array('checked' => FALSE),
          ),
        ),
      ),
      'help' => array(
        '#markup' => t('Specify which content models this block should be applied to, and which Solr fields designate the children of these content models. Use "%pid" as a replacement value for the PID of the parent object; the colon in the PID will be escaped to an underscore (e.g., isSequenceNumberOf%pid becomes isSequenceNumberOfnamespace_id).'),
      ),
      'islandora_solr_table_of_contents_content_models' => array(
        '#tree' => TRUE,
        '#theme' => 'islandora_solr_table_of_contents_block_management_table',
        '#header' => $header,
        'rows' => $options,
      ),
    );
  }
  return array();
}

/**
 * Checks if a thing is a string with content, and yells if it isn't.
 *
 * @param string $element
 *   The string to check.
 */
function islandora_solr_table_of_contents_validate_string($element) {
  if ($element['#value'] == '') {
    form_set_error('islandora_solr_table_of_contents', t("Please fill out the '@title' box.", array('@title' => $element['#title'])));
  }
}

/**
 * Updates and determines where to start the block tree from.
 *
 * @param AbstractObject $object
 *   The object on the current page.
 *
 * @return AbstractObject
 *   The object to start from.
 */
function islandora_solr_table_of_contents_refresh_block_start_point($object) {
  if (variable_get('islandora_solr_table_of_contents_save_state', TRUE)) {
    $root_node = variable_get('islandora_solr_table_of_contents_root_node', 'last_collection');
    // Cache this PID if it's a collection and that's what we do.
    if ($root_node == 'last_collection' && in_array('islandora:collectionCModel', $object->models)) {
      $_SESSION['islandora_solr_table_of_contents_cached_pid'] = $object->id;
    }

    // Determine the PID to start the block from. From there, collapsed nodes
    // are determined by the state, if applicable.
    // That makes it kind of sound like the government is micromanaging your
    // collapsed nodes. It's just your browser session storage, don't worry.
    switch (variable_get('islandora_solr_table_of_contents_root_node', 'last_collection')) {
      case 'root_collection':
        return islandora_object_load('islandora:root');

      case 'last_collection':
        if (isset($_SESSION['islandora_solr_table_of_contents_cached_pid'])) {
          return $_SESSION['islandora_solr_table_of_contents_cached_pid'] == $object->id ? $object : islandora_object_load($_SESSION['islandora_solr_table_of_contents_cached_pid']);
        }

      default:
        return $object;
    }
  }
  return $object;
}

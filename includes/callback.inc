<?php

/**
 * @file
 * Callback functionality for table of contents.
 */

/**
 * Outputs JSON representing the children of an object using Solr.
 *
 * @param AbstractObject $object
 *   The object to get children for.
 */
function islandora_solr_table_of_contents_get_jstree_list(AbstractObject $object) {
  module_load_include('inc', 'islandora_solr_table_of_contents', 'includes/utilities');
  $children = islandora_solr_table_of_contents_get_children($object);
  // Models with Children sounds like a depressing tumblr account.
  $model_config = variable_get('islandora_solr_table_of_contents_content_models', islandora_solr_table_of_contents_get_default_configuration());
  $filter = function($key) {
    return isset($key['solr_field']);
  };
  $models_with_children = array_keys(array_filter($model_config, $filter));

  // Oh boy, here I go listin' again!
  $master_list = array();
  foreach ($children as $child) {
    $child_data = array(
      'id' => $child['PID'],
      'text' => $child['label'],
      'a_attr' => array(
        'href' => url("islandora/object/{$child['PID']}"),
      ),
    );
    // If this content model potentially has children, we should make this a
    // collapsible, collapsed node.
    if (array_intersect($child_models, $models_with_children)) {
      $child_data['children'] = TRUE;
    }
    $master_list[] = $child_data;
  }
  drupal_json_output($master_list);
  drupal_exit();
}

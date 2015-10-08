<?php

/**
 * @file
 * Callback functionality for table of contents.
 */

/**
 * Returns the children of an object using configured Solr fields/sorting.
 *
 * @param AbstractObject $object
 *   The object to get children for.
 *
 * @return string
 *   Some fancy JSON.
 */
function islandora_solr_table_of_contents_get_jstree_list(AbstractObject $object, $json = TRUE) {
  global $base_url;
  module_load_include('inc', 'islandora_solr_table_of_contents', 'includes/utilities');
  $children = islandora_solr_table_of_contents_get_children($object);
  // Models with Children sounds like a depressing tumblr account.
  $model_config = variable_get('islandora_solr_table_of_contents_content_models', islandora_solr_table_of_contents_get_default_configuration());
  $models_with_children = array_map(function($key) {
    return isset($key['solr_field']);
  }, $model_config);

  // Oh boy, here I go listin' again!
  $master_list = array();
  foreach ($children as $child) {
    $pid = str_replace(':', '-', $child['PID']);
    $child_data = array(
      'id' => $child['PID'],
      'text' => $child['label'],
      'a_attr' => array(
        'href' => "/islandora/object/{$child['PID']}",
      ),
    );
    // If this content model potentially has children, we should make this a
    // collapsible, collapsed node.
    foreach ($child['models'] as $model) {
      if (in_array($model, array_keys($models_with_children)) && $models_with_children[$model]) {
        $child_data['children'] = TRUE;
      }
    }
    $master_list[] = $child_data;
  }
  drupal_json_output($master_list);
  drupal_exit();
}

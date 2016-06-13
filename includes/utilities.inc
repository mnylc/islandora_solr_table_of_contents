<?php

/**
 * @file
 * Utility functionality.
 */

/**
 * Gets children of an object from Solr.
 *
 * @param AbstractObject $object
 *   The object in question.
 *
 * @return array|bool
 *   An array of children arrays containing 'PID' strings, 'label' strings, and
 *   'models' arrays, or FALSE on failure.
 */
function islandora_solr_table_of_contents_get_children(AbstractObject $object) {
  // Some variables we'll need.
  $has_model = variable_get('islandora_solr_content_model_field', 'RELS_EXT_hasModel_uri_ms');
  $label = variable_get('islandora_solr_object_label_field', 'fgs_label_s');
  $config = islandora_solr_table_of_contents_get_object_config($object);
  if (empty($config)) {
    return array();
  }

  // Start building the query to send off.
  $p = new IslandoraSolrQueryProcessor();
  $limit = variable_get('islandora_solr_table_of_contents_max_objects', 1000);
  $params = array(
    'fl' => "PID, $has_model, $label",
    'limit' => $limit,
  );
  if (isset($config['sort_field'])) {
    $params['sort'] = islandora_solr_table_of_contents_add_pid_to_field($object->id, $config['sort_field']) . " asc";
  }
  $query = format_string('!relationship_field:("info:fedora/!pid" OR "!pid")', array(
    '!relationship_field' => $config['solr_field'],
    '!pid' => $object->id,
  ));
  $p->buildAndExecuteQuery($query, $params, FALSE);

  // Build the results array.
  if (isset($p->islandoraSolrResult['response']['objects'])) {
    $results = array();
    foreach ($p->islandoraSolrResult['response']['objects'] as $result) {
      $results[$result['PID']] = array(
        'PID' => $result['solr_doc']['PID'],
        'label' => isset($result['solr_doc'][$label]) ? $result['solr_doc'][$label] : $result['solr_doc']['PID'],
        'models' => (array) isset($result['solr_doc'][$has_model]) ? $result['solr_doc'][$has_model] : array(),
      );
      foreach ($results[$result['PID']]['models'] as &$model) {
        $model = str_replace('info:fedora/', '', $model);
      }
    }
    return $results;
  }
  return FALSE;
}

/**
 * Returns a version of the field with %pid replaced by a PID.
 *
 * Colons in PIDs are replaced with underscores.
 *
 * @param string $pid
 *   The PID to insert.
 * @param string $field
 *   The field to insert it into.
 *
 * @return string
 *   The modified string.
 */
function islandora_solr_table_of_contents_add_pid_to_field($pid, $field) {
  $pid = strtolower(str_replace(':', '_', $pid));
  return str_replace('%pid', $pid, $field);
}

/**
 * Gets the config for a given object's content model.
 *
 * @param AbstractObject $object
 *   The object to get the config for.
 *
 * @return array|bool
 *   The configuration array containing 'solr_field' and possibly 'sort_field',
 *   or FALSE if no configuration was found.
 */
function islandora_solr_table_of_contents_get_object_config(AbstractObject $object) {
  $config = variable_get('islandora_solr_table_of_contents_content_models', islandora_solr_table_of_contents_get_default_configuration());
  $content_models = array_intersect(array_keys($config), $object->models);
  if (!empty($content_models)) {
    $content_model = array_shift($content_models);
    return $config[$content_model];
  }
  return FALSE;
}

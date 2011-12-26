<?php

/**
 * @file
 * Admin page callbacks for the xboxgames module.
 *
 * @author Jon Kamke
 */

/**
 * Implements hook_form()
 * @param form
 * @param form_state
 *
 * Form function, called by drupal_get_form()
 * in xboxgames_menu().
 */
function xboxgames_form($form, &$form_state) {
  $form['xboxgames_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#default_value' => variable_get('xboxgames_api_key'),
    '#size' => 32,
    '#maxlength' => 32,
    '#description' => t('The API key to access SOAP webservices.'),
    '#required' => TRUE,
  );

  $form['submit']['apikey'] = array('#type' => 'submit', '#value' => t('Submit Key'));

  $form['submit']['clearGames'] = array('#type' => 'submit', '#value' => t('Clear Games'));

  return $form;
}

/**
 *
 * Save admin settings when form is submitted
 * @param $form_id
 * @param $form_state
 */
function xboxgames_form_submit($form_id, &$form_state) {

  if($form_state['clicked_button']['#value'] == 'Submit Key') {
      // wsclient module
      $service = wsclient_service_load('xbox_voting');
      $result = $service->checkKey($form_state['values']['xboxgames_api_key']);
      if($result) {
          //Save apikey as a variable
          variable_set('xboxgames_api_key', $form_state['values']['xboxgames_api_key']);

          drupal_set_message(t('API key saved.'), 'status');
      } else {
          drupal_set_message(t('That was not a valid API key.'), 'error');
      }


  }
  else if($form_state['clicked_button']['#value'] == 'Clear Games') {
    //clear list of games
    // wsclient module
      $service = wsclient_service_load('xbox_voting');
      $result = $service->clearGames(variable_get('xboxgames_api_key'));

      if($result) {
          drupal_set_message(t('Xbox games cleared successfully'), 'status');
      } else {
          drupal_set_message(t('There was a problem clearing Xbox games.'), 'error');
      }


  }

}
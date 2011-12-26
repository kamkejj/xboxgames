<?php

/**
 *@file
 * New game form callbacks, saves new game titles
 *
 * @author Jon Kamke
 */

/**
 * Form to add new game titles to be voted on
 * @param array $form
 * @param array $form_state
 * @return array
 */
function xboxgames_newgame_form($form, &$form_state)  {
    $form['xboxgames_new_game_title'] = array(
    '#type' => 'textfield',
    '#title' => t('New Xbox Game'),
    '#size' => 32,
    '#maxlength' => 64,
    '#description' => t('Add a new game to be voted on.'),
    '#required' => TRUE,
  );

  $form['submit']['new_game'] = array('#type' => 'submit', '#value' => t('Submit Game'));

  return $form;
}

/**
 *  Validate user input when adding a new game title
 * @param array $form
 * @param array $form_state
 */
function xboxgames_newgame_form_validate($form, &$form_state) {
    global $user;
    $xbox = new Xboxgames();

    // if user already voted set error and stop form submission
    if($xbox->hasVoted($user->uid, REQUEST_TIME)) {
         form_set_error('xboxgames_new_game_title', t('You already voted today or added a new game title.'));

         // wsclient module
        $service = wsclient_service_load('xbox_voting');
        $result = $service->getGames(variable_get('xboxgames_api_key'));

        // if duplicate game title is found ser error and stop form submission
        if($xbox->isDupTitle($result, $form_state['values']['xboxgames_new_game_title']))
               form_set_error('xboxgames_new_game_title', t('Game title already exists.'));
    }
}

/**
 *
 * Submit form to add new game title
 *  User can submit new title and no duplicate game titles
 * @param array $form_id
 * @param array $form_state
 */
function xboxgames_newgame_form_submit($form_id, &$form_state) {
    global $user;
    $xbox = new Xboxgames();

    if($form_state['clicked_button']['#value'] == 'Submit Game') {

      //Check if user has voted today
      $voted = $xbox->hasVoted($user->uid, REQUEST_TIME);
      if($voted) {//allready voted today
          drupal_set_message(t('You already voted or added a game title today.'), 'warning');
      } else { //has not voted today
           //adding game title counts as a vote, mark that user has voted
           $insert_result = $xbox->addVote($user->uid, REQUEST_TIME);

           // wsclient module
          $service = wsclient_service_load('xbox_voting');
          $title_result = $service->addGame(variable_get('xboxgames_api_key'),$form_state['values']['xboxgames_new_game_title']);

          // if vote was saved successfully and new title was saved
          if($title_result && $insert_result) {
              drupal_set_message(t('New game title added.'), 'status');
          } else {
              //something went wrong saving the vote or adding the game title
              drupal_set_message(t('There was a problem adding the game title.'), 'error');
          }
    }
  }
}

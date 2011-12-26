<?php
/**
 * @file
 * Xbox game voting apllication class
 *
 * @author Jon Kamke
 */

class Xboxgames {

function __construct() {

}

/**
 * Compare votes to sort in descending order
 * @param obj $a
 * @param obj $b
 * @return int
 */
static function cmpVotes($obja, $objb) {
  $a = (int)$obja->votes;
  $b = (int)$objb->votes;
  if($a == $b){ return 0 ; }

  return ($a > $b) ? -1 : 1;
}

/**
 * Sort games by votes in descending order
 * @param array $arr
 * @return array
 */
function sortVotes($arr) {
    usort($arr,'self::cmpVotes');

    return $arr;
}

/**
 * Compare game titles to sort in ascending order
 * @param $array $obja
 * @param $array $objb
 * @return int
 */
static function cmpTitle($obja, $objb) {

      if($obja->title == $objb->title) { return 0 ; }

      return ($obja->title < $objb->title) ? -1 : 1;
}

/**
 * Sort games by titles in ascending order
 * @param array $arr
 * @return array
 */
function sortTitle($arr) {
    usort($arr, 'self::cmpTitle');

    return $arr;
}

/**
 *  Return TRUE if game is set as wantit
 * @param obj $obj
 * @return boolean
 */
function isWantIt($obj) {
    if($obj->status == 'wantit')
           return TRUE;

    return FALSE;
}

/**
 * Return TRUE if game is set as gotit
 * @param obj $obj
 * @return boolean
 */
function isGotIt($obj) {
        if($obj->status == 'gotit')
           return TRUE;

    return FALSE;
}

/**
 * Check for duplicate game titles
 * @param array $arr
 * @param string $title
 * @return boolean Return TRUE if game titles match or FALSE if not
 */
function isDupTitle($arr, $title) {
    foreach($arr as $game) {
        if(strtolower($game->title) == strtolower($title))//game title already exists using lower case
            return TRUE;
    }

    return FALSE;
}

/**
 * Check if user already voted
 *  Adding a new game title equals a vote
 * @param int $uid
 * @param int $today
 * @return boolean TRUE if created matches todays date or FALSE if no match
 */
 function hasVoted($uid, $today) {
    //Use Database API to retrieve a user.
    $query = db_select('xboxgames_vote', 'xv')
    ->fields('xv', array('uid', 'created'))
    ->condition('uid', $uid, '=') //user id.
    ->orderBy('created', 'DESC') //get most recent by created date
    ->range(0,1)
    ->execute()
    ->fetchObject();

    //if user id was found compare created date with todays date
    if(!empty($query))
      if(date('d/m/Y',$query->created) == date('d/m/Y', $today))
        return TRUE;

    return FALSE;
  }

  /**
   * Saves user id and todays date to vote table
   *  Marks user as having voted or added a new game title today
   * @param int $uid
   * @param int $date
   * @return type
   */
  function addVote($uid, $date) {
        //insert uid, created date to vote table
        //Create an array of form fields to use in query
        $fields = array(
            'uid' =>  $uid,
            'created' => $date,
        );
        $insert = db_insert('xboxgames_vote')
        ->fields($fields)
        ->execute();

        return $insert;
  }

  /**
   *
   * @param array $arr
   * @return array
   */
  function removeGame($arr) {

      $i = 0;
      foreach($arr as $game) {
          if($game->status == 'gotit') {
              unset($arr[$i]);
          }
          $i++;
      }

      return $arr;
  }

}
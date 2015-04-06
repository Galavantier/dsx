<?php

/**
 * Class DsxVar
 * A boxed var class that keeps a custom variable table so that you can pass variable references down through DSX components.
 */
class DsxVar {
  private static $varTable = array();

  private $varID;

  public function __construct($var) {
    if(self::isDsxVar($var)) {
      $this->varID = self::getVarRef($var);
    } else {
      $this->varID = uniqid();
      self::$varTable[$this->varID] = $var;
    }
  }

  public function __toString() {
    return "DsxRef/" . $this->varID . "/";
  }

  public function __invoke() {
    return $this->get();
  }

  public function value() {
    return $this->get();
  }

  public function get() {
    return self::$varTable[$this->varID];
  }

  private static function getVarRef($refStr) {
    $ref = explode('/', $refStr);
    return $ref[1];
  }

  public static function isDsxVar($str) {
    return (preg_match('/DsxRef\/([a-z|A-Z|0-9]+)\//',(string)$str) === 1);
  }

}

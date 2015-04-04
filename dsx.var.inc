<?php

/**
 * Class DsxVar
 * A boxed var class that keeps a custom variable table so that you can pass variable references down through DSX components.
 */
class DsxVar {
  private static $varTable = array();

  private $varID;

  public function __construct($var) {
    $this->varID = uniqid();
    self::$varTable[$this->varID] = $var;
  }

  public function __toString() {
    return "DsxRef/" . $this->varID . "/";
  }

  public function __invoke($var = false) {
    return (!$var) ? $this->get() : $this->set($var);
  }

  public static function getVarRef($refStr) {
    $ref = explode('/', $refStr);
    return self::$varTable[$ref[1]];
  }

  public function get() {
    return self::$varTable[$this->varID];
  }

  public function set($var) {
    self::$varTable[$this->varID] = $var;
    return self::$varTable[$this->varID];
  }
}

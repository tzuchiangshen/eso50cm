CREATE DATABASE logs;
GRANT ALL PRIVILEGES ON logs.* TO 'logmgr' IDENTIFIED BY 'logpass'; FLUSH PRIVILEGES;
DROP TABLE  `logs`.`LOG_SOURCES` ;
DROP TABLE  `logs`.`LOGS` ;

CREATE TABLE `logs`.`LOG_SOURCES` (
  `SOURCE_ID` INT  NOT NULL AUTO_INCREMENT,
  `SOURCE_DESCRIPTION` VARCHAR(128)  NOT NULL,
  `DISCARD_LEVEL` INT  NOT NULL DEFAULT 2,
  PRIMARY KEY (`SOURCE_ID`, `SOURCE_DESCRIPTION`)
)
ENGINE = InnoDB;

CREATE TABLE `logs`.`LOGS` (
  `LOG_ID` INT  NOT NULL AUTO_INCREMENT,
  `LEVEL` INT  NOT NULL,
  `SOURCE_ID` INT  NOT NULL,
  `TIMESTAMP` DOUBLE  NOT NULL,
  `DATA` VARCHAR(2048) ,
  PRIMARY KEY (`LOG_ID`),
  CONSTRAINT `sourceid_fk_constraint` FOREIGN KEY `sourceid_fk_constraint` (`SOURCE_ID`)
    REFERENCES `LOG_SOURCES` (`SOURCE_ID`)
    ON DELETE RESTRICT
    ON UPDATE RESTRICT
)
ENGINE = InnoDB;


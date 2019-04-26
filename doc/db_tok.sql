DROP TABLE IF EXISTS `t_offline_message`;
CREATE TABLE `t_offline_message` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `local_msg_id` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'local msg id, generate by client',
  `fr_pk` char(64) NOT NULL DEFAULT '' COMMENT 'sender public key',
  `to_pk` char(64) NOT NULL DEFAULT '' COMMENT 'receiver public key',
  `content` varbinary(2048) NOT NULL DEFAULT '' COMMENT 'msg content',
  `create_time` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'msg create time',
  PRIMARY KEY (`id`),
  UNIQUE KEY `index_local_msg_id` (`local_msg_id`)
  )ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT 'offline message';
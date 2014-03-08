ALTER TABLE `saved_variables` ADD COLUMN `NextRandomBGResetTime` BIGINT(40) UNSIGNED NOT NULL DEFAULT '0' AFTER `NextWeeklyQuestResetTime`;

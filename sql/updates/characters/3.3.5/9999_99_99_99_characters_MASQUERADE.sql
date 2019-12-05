-- Add new column used by masquerade system
ALTER TABLE `characters` ADD COLUMN IF NOT EXISTS `masqueradeRace` tinyint(3) unsigned NOT NULL DEFAULT '0' AFTER `knownTitles`;

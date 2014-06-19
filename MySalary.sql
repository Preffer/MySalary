-- phpMyAdmin SQL Dump
-- version 4.0.10deb1
-- http://www.phpmyadmin.net
--
-- 主机: localhost
-- 生成日期: 2014-06-20 00:23:10
-- 服务器版本: 5.5.37-MariaDB-0ubuntu0.14.04.1
-- PHP 版本: 5.5.9-1ubuntu4

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- 数据库: `qt`
--

-- --------------------------------------------------------

--
-- 表的结构 `admin`
--

CREATE TABLE IF NOT EXISTS `admin` (
  `adminID` int(11) NOT NULL,
  `privilege` int(11) NOT NULL,
  `staffID` int(11) NOT NULL,
  PRIMARY KEY (`adminID`),
  KEY `staffID` (`staffID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- 转存表中的数据 `admin`
--

INSERT INTO `admin` (`adminID`, `privilege`, `staffID`) VALUES
(1, 100, 1),
(2, 92, 2);

-- --------------------------------------------------------

--
-- 表的结构 `bonus`
--

CREATE TABLE IF NOT EXISTS `bonus` (
  `bonusID` int(11) NOT NULL AUTO_INCREMENT,
  `staffID` int(11) NOT NULL,
  `effect` float NOT NULL,
  `note` text NOT NULL,
  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`bonusID`),
  KEY `bonus_ibfk_1` (`staffID`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=4 ;

--
-- 转存表中的数据 `bonus`
--

INSERT INTO `bonus` (`bonusID`, `staffID`, `effect`, `note`, `timestamp`) VALUES
(2, 1, 123, 'hurry up', '2014-06-16 18:14:48'),
(3, 1, -10, '33', '2014-06-16 18:15:23');

-- --------------------------------------------------------

--
-- 表的结构 `grade`
--

CREATE TABLE IF NOT EXISTS `grade` (
  `gradeID` int(11) NOT NULL,
  `gradeName` varchar(64) NOT NULL,
  `baseSalary` float NOT NULL,
  PRIMARY KEY (`gradeID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- 转存表中的数据 `grade`
--

INSERT INTO `grade` (`gradeID`, `gradeName`, `baseSalary`) VALUES
(37, '3333', 33),
(44, '44', 44),
(50, 'New', 495),
(100, 'Root', 100000);

-- --------------------------------------------------------

--
-- 表的结构 `salary`
--

CREATE TABLE IF NOT EXISTS `salary` (
  `staffID` int(11) NOT NULL,
  `date` date NOT NULL,
  `salary` float NOT NULL,
  PRIMARY KEY (`staffID`,`date`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- 转存表中的数据 `salary`
--

INSERT INTO `salary` (`staffID`, `date`, `salary`) VALUES
(1, '2014-05-01', 1000),
(1, '2014-06-01', 1000),
(1, '2014-06-15', 100113),
(1, '2014-06-18', 100113),
(1, '2014-08-18', 100000),
(1, '2014-09-18', 100000),
(2, '2014-06-15', 44),
(2, '2014-06-18', 44),
(2, '2014-08-18', 495),
(2, '2014-09-18', 495),
(3, '2014-08-18', 33),
(3, '2014-09-18', 33),
(6, '2014-08-18', 495),
(6, '2014-09-18', 495),
(8, '2014-06-15', 44),
(8, '2014-06-18', 44);

-- --------------------------------------------------------

--
-- 表的结构 `staff`
--

CREATE TABLE IF NOT EXISTS `staff` (
  `staffID` int(11) NOT NULL,
  `name` varchar(64) NOT NULL,
  `info` text NOT NULL,
  `gradeID` int(11) NOT NULL,
  `loginName` varchar(64) NOT NULL,
  `password` text NOT NULL,
  PRIMARY KEY (`staffID`),
  UNIQUE KEY `loginName_2` (`loginName`),
  KEY `gradeID` (`gradeID`),
  KEY `loginName` (`loginName`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- 转存表中的数据 `staff`
--

INSERT INTO `staff` (`staffID`, `name`, `info`, `gradeID`, `loginName`, `password`) VALUES
(1, '黄羽众', 'uuuyy', 100, 'hyz', '41d97610f9fab3dbe1ad81633f02b6cd'),
(2, 'who', 'who', 50, 'who', '53d670af9bb16ea82e7ef41ee23ec6df'),
(3, 'yk', 'yk', 37, 'yk', ''),
(6, 'foo', 'foo', 50, 'foo', 'acbd18db4cc2f85cedef654fccc4a4d8');

--
-- 限制导出的表
--

--
-- 限制表 `admin`
--
ALTER TABLE `admin`
  ADD CONSTRAINT `admin_ibfk_1` FOREIGN KEY (`staffID`) REFERENCES `staff` (`staffID`);

--
-- 限制表 `bonus`
--
ALTER TABLE `bonus`
  ADD CONSTRAINT `bonus_ibfk_1` FOREIGN KEY (`staffID`) REFERENCES `staff` (`staffID`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- 限制表 `staff`
--
ALTER TABLE `staff`
  ADD CONSTRAINT `staff_ibfk_1` FOREIGN KEY (`gradeID`) REFERENCES `grade` (`gradeID`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

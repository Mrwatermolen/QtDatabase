#ifndef _COMMON_H_
#define _COMMON_H_

#include <QtCore>
namespace DatabaseTableConfig {
/**
 * @brief 字段是否在表中显示
 *
 */
enum FieldStatus { InTableView, InMenu, Disable };

/**
 * @brief 字段类型
 *
 */
enum FieldType {
  String,
  Text,
  Integer,
  Double,
  DateTime,
  PrimaryKey,
  ForeignKey,
  Others
};

// 一个映射 TODO: 不怎么好看
const QMap<QString, FieldStatus>
    StrToStatus({std::make_pair("inTableView", FieldStatus::InTableView),
                 std::make_pair("inMenu", FieldStatus::InMenu),
                 std::make_pair("disable", FieldStatus::Disable)});

const QMap<QString, FieldType> StrToType({
    std::make_pair("String", FieldType::String),
    std::make_pair("Text", FieldType::Text),
    std::make_pair("Integer", FieldType::Integer),
    {"Double", FieldType::Double},
    {"DateTime", FieldType::DateTime},
    {"ForeignKey", FieldType::ForeignKey},
    {"Others", FieldType::Others},
});

/**
 * @brief 字段配置结构体
 *
 */
struct FieldConfig {
  QString field;
  QString name;
  FieldStatus status;
  FieldType type{String};

  bool operator==(const FieldConfig &c) const {
    if (&c == this) {
      return true;
    }
    if (field != c.field || name != c.name || status != c.status ||
        type != c.type) {
      return false;
    }
    return true;
  }
  bool operator!=(const FieldConfig &c) const { return !operator==(c); }
};

/**
 * @brief 表格配置结构体 记录字段的显示状态
 *
 */
struct TableConfig {
  QString tableName;
  QVector<FieldConfig> fieldConfig;
  int version{-1};
  bool operator==(const TableConfig &c) const {
    if (&c == this) {
      return true;
    }
    if (tableName != c.tableName || fieldConfig != c.fieldConfig ||
        version != c.version) {
      return false;
    }
    return true;
  }
  bool operator!=(const TableConfig &c) const { return !operator==(c); }
};

struct TableFilterConfig {
  QVector<FieldConfig> fieldConfig;
  QVector<QVariant> data;
};
} // namespace DatabaseTableConfig

/**
 * @brief 数据交换结构体
 *
 */
struct DataStream {
  DatabaseTableConfig::FieldConfig fieldConfig;
  QVariant data;
};

#endif // _COMMON_H_
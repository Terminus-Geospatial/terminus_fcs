# FCS Property System

The FCS (Flight Control System) library provides a flexible, tree-based property management system with schema validation and dot-notation access.

## Overview

The Property system consists of three main components:

1. **Property Classes** - Hierarchical property storage with type safety
2. **PropertySchema** - Validation and constraint system
3. **Datastore** - Tree management and command-line parsing

## Key Features

- **Tree-like Structure**: Properties can be nested using dot notation (e.g., `config.database.host`)
- **Type Safety**: Strongly typed properties (string, integer, float, boolean, array, object)
- **Schema Validation**: Define constraints and validation rules for properties
- **Command-Line Parsing**: Parse `-p key=value` style arguments
- **Error Handling**: Comprehensive error reporting using Terminus Result types

## Usage Examples

### Basic Property Creation

```cpp
#include <terminus/fcs/datastore.hpp>
#include <terminus/fcs/property_schema.hpp>

// Create a datastore
Datastore datastore;

// Parse command-line arguments
std::vector<std::string> args = {
    "-pkey1.key2.key3=value123",
    "-pconfig.database.host=localhost",
    "-pconfig.database.port=5432"
};
datastore.parse_command_line_args(args);
```

### Setting and Getting Properties

```cpp
// Set a property directly
datastore.set_property("server.workers", 4);

// Get a property
auto prop = datastore.get_property("config.database.host");
if (prop) {
    auto string_prop = std::dynamic_pointer_cast<StringProperty>(prop.value());
    if (string_prop) {
        auto value = string_prop->get_typed_value();
        if (value) {
            std::cout << "Host: " << value.value() << std::endl;
        }
    }
}
```

### Schema Validation

```cpp
// Create a schema for database configuration
auto db_schema = Schema_Builder(Property_Value_Type::OBJECT)
    .required(true)
    .description("Database configuration")
    .property("host", Builder(Property_Value_Type::STRING)
        .required(true)
        .description("Database hostname")
        .build())
    .property("port", Builder(Property_Value_Type::INTEGER)
        .required(true)
        .range(1, 65535)
        .description("Database port")
        .build())
    .build();

// Apply and validate schema
datastore.set_schema("config.database", db_schema);
auto validation_result = datastore.validate_property("config.database");
```

## Property Types

### Primitive Types
- **StringProperty**: Text values
- **IntegerProperty**: 64-bit integer values
- **FloatProperty**: Double-precision floating point values
- **BooleanProperty**: True/false values

### Container Types
- **ObjectProperty**: Nested object containing other properties
- **ArrayProperty**: Ordered list of properties

## Schema Constraints

### Range Constraints
```cpp
SchemaBuilder(PropertyValueType::INTEGER)
    .range(1, 100)  // Value must be between 1 and 100
    .build();
```

### Enum Constraints
```cpp
SchemaBuilder(PropertyValueType::STRING)
    .enum_values({"small", "medium", "large"})
    .build();
```

### Custom Constraints
```cpp
auto validator = [](const std::any& value) -> Result<void> {
    // Custom validation logic
    return ok<void>();
};

SchemaBuilder(PropertyValueType::STRING)
    .custom(std::make_shared<CustomConstraint>(validator, "Custom validation"))
    .build();
```

## Error Handling

The system uses Terminus Result types for comprehensive error handling:

```cpp
auto result = datastore.get_property("some.path");
if (!result) {
    std::cerr << "Error: " << result.error().message() << std::endl;
    // Handle error...
}
```

### Error Types
- `PROPERTY_NOT_FOUND`: Requested property doesn't exist
- `TYPE_MISMATCH`: Value type doesn't match expected type
- `INVALID_PATH`: Malformed property path
- `VALIDATION_FAILED`: Schema validation failed
- `PARSING_ERROR`: Command-line argument parsing failed

## Tree Navigation

```cpp
// Navigate to an object property
auto config = datastore.get_property("config");
if (config) {
    auto obj = std::dynamic_pointer_cast<ObjectProperty>(config.value());
    if (obj) {
        // List children
        auto children = obj->get_child_keys();

        // Access specific child
        auto db = obj->get_property("database");
    }
}
```

## Command-Line Integration

The datastore can parse command-line arguments in the format:
```
-p key1.key2.key3=value
```

Multiple `-p` arguments can be processed, and the system will automatically:
- Create nested object properties as needed
- Infer property types from string values
- Build the property tree structure

## Thread Safety

The current implementation is **not thread-safe**. If you need concurrent access, implement appropriate synchronization mechanisms.

## Performance Considerations

- Property lookup is O(log n) for the path depth
- Schema validation is performed on-demand
- Memory usage scales with the number of properties
- Consider using property references for frequent access patterns

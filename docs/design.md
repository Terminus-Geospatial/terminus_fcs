# Framework Configuration Service Design

## Overview

The Framework Configuration Service (FCS) is a centralized configuration management system designed to provide a unified way to manage application settings, environment variables, and feature flags across the Terminus framework.

It contains 2 aspects:  a C++ library and an application service.  The C++ library provides the core configuration API while the application service provides the user with an interface for managing the configuration through a CLI or web interface.


## Key Features

- Centralized configuration storage and retrieval
- Support for multiple configuration sources (config files, command-line arguments, and environment variables)
- Real-time configuration updates without application restart
- Type-safe configuration access with validation
- Schema validation for configuration structures
- Feature flag management
- Configuration versioning and rollback capabilities
- Connect over terminus-ipc API to share configuration across processes

## Architecture

The FCS follows a modular architecture with the following components:

### Core Components

- **Datastore API**: `datastore.hpp`
    - Provides a unified interface for configuration storage and retrieval
    - Supports CRUD operations for configuration entities
    - Handles configuration validation and type conversion
    - Manages configuration change notifications and listeners
    - Thread-safe operations for concurrent access

- **Configuration Loader**: `configuration.hpp`
    - Loads configuration from multiple sources in priority order
    - Sources: files > command-line > environment variables > defaults
    - Merges configurations with conflict resolution

- **Service API**
    - Contains a Datastore for managing the underlying state
    - Listens and communicates over a Terminus Service IPC API
    - Provides remote configuration access

### Data Types

- **Property_Base**
    - Abstract base class for all configuration properties
    - Provides type-erased interface for property storage
    - Supports serialization/deserialization
    - Metadata: description, required/optional, default value

- **Schema_Property**
    - Typed property with validation rules
    - Supports constraints (min/max, regex, enums)
    - Automatic type conversion and validation
    - Schema definition support

### Key Classes

```cpp
namespace tmns::fcs {

// Core property types
class Property_Base;
template<typename T> class Schema_Property;

// Main datastore
class Datastore {
public:
    // CRUD operations
    Result<void> set_property(const std::string& key, std::shared_ptr<Property_Base> value);
    Result<std::shared_ptr<Property_Base>> get_property(const std::string& key) const;
    Result<void> remove_property(const std::string& key);

    // Schema operations
    Result<void> register_schema(const std::string& key, std::shared_ptr<Schema_PropertyBase> schema);
    Result<void> validate_against_schema() const;

    // Change notifications
    using ChangeCallback = std::function<void(const std::string& key, const Property_Base& value)>;
    void add_change_listener(const std::string& key_pattern, ChangeCallback callback);

    // Persistence
    Result<void> load_from_file(const std::filesystem::path& path);
    Result<void> save_to_file(const std::filesystem::path& path) const;
};

// Configuration loader
class ConfigurationLoader {
public:
    static Result<Datastore> load_from_sources(int argc, char* argv[], char* envp[]);
    static Result<Datastore> load_from_file(const std::filesystem::path& path);
};

}
```

## Implementation Questions

1. **Property Types**: What basic types should be supported initially? (string, int, double, bool, arrays, objects?)

2. **Configuration Format**: What file format should be supported? (JSON, YAML, TOML, or custom?)

3. **IPC Integration**: How should the service API integrate with the existing Terminus IPC system?

4. **Thread Safety**: What level of concurrency protection is needed? Reader-writer locks?

5. **Validation**: Should validation be strict (reject invalid) or permissive (warn but accept)?

6. **Change Notifications**: Should notifications be synchronous or asynchronous?

7. **Schema Definition**: How should schemas be defined? Code-based, file-based, or both?

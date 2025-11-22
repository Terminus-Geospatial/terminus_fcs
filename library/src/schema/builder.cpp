
// SchemaBuilder implementation
SchemaBuilder::SchemaBuilder(PropertyValueType type) {
    m_schema = std::make_shared<PropertySchema>(type);
}

SchemaBuilder& SchemaBuilder::required(bool is_required) {
    m_schema->set_required(is_required);
    return *this;
}

SchemaBuilder& SchemaBuilder::default_value(const std::any& value) {
    m_schema->set_default_value(value);
    return *this;
}

SchemaBuilder& SchemaBuilder::description(const std::string& desc) {
    m_schema->set_description(desc);
    return *this;
}

SchemaBuilder& SchemaBuilder::range(double min_val, double max_val) {
    auto constraint = std::make_shared<RangeConstraint<double>>(min_val, max_val);
    m_schema->add_constraint(constraint);
    return *this;
}

SchemaBuilder& SchemaBuilder::range(int64_t min_val, int64_t max_val) {
    auto constraint = std::make_shared<RangeConstraint<int64_t>>(min_val, max_val);
    m_schema->add_constraint(constraint);
    return *this;
}

SchemaBuilder& SchemaBuilder::enum_values(const std::vector<std::string>& values) {
    auto constraint = std::make_shared<EnumConstraint>(values);
    m_schema->add_constraint(constraint);
    return *this;
}

SchemaBuilder& SchemaBuilder::custom(std::shared_ptr<ValidationConstraint> constraint) {
    m_schema->add_constraint(constraint);
    return *this;
}

SchemaBuilder& SchemaBuilder::property(const std::string& key, std::shared_ptr<PropertySchema> schema) {
    m_schema->add_property_schema(key, schema);
    return *this;
}

SchemaBuilder& SchemaBuilder::items(std::shared_ptr<PropertySchema> schema) {
    m_schema->set_item_schema(schema);
    return *this;
}

std::shared_ptr<PropertySchema> SchemaBuilder::build() {
    return m_schema;
}

// Utility functions
std::shared_ptr<PropertySchema> create_string_schema() {
    return SchemaBuilder(PropertyValueType::STRING).build();
}

std::shared_ptr<PropertySchema> create_integer_schema() {
    return SchemaBuilder(PropertyValueType::INTEGER).build();
}

std::shared_ptr<PropertySchema> create_float_schema() {
    return SchemaBuilder(PropertyValueType::FLOAT).build();
}

std::shared_ptr<PropertySchema> create_boolean_schema() {
    return SchemaBuilder(PropertyValueType::BOOLEAN).build();
}

std::shared_ptr<PropertySchema> create_object_schema() {
    return SchemaBuilder(PropertyValueType::OBJECT).build();
}

std::shared_ptr<PropertySchema> create_array_schema() {
    return SchemaBuilder(PropertyValueType::ARRAY).build();
}
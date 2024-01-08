package body record_clash is
  function compare_records return Boolean is
  begin
    record3.fieldA.field1 := 5;
    return record1.field1 = record2.field1
       and record1.field2 = record2.field2
       and record1.field3 = record2.field3;
  end compare_records;
end record_clash;

-- If fields are identified as attributes: 6 attributes.
-- If only whole records are attributes: 2 attributes.
-- If records and fields are distinct attributes: 5 attributes.

-- Ways to handle records:

-- Example record addresses (unique IDs).
--  record1: 0x01
--  record2: 0x02
--  field1:  0x03
--  field2:  0x04
--  field3:  0x05

-- Ignore fields. Any field access is treated as considering the whole record.
--  record1.field1: 0x01
--  record2.field1: 0x02
--  record2: 0x02

-- Consider each unique record-field pair as an attribute.
-- This can be done by making attributes a vector of SgInitializedName*.
--  record1.field1: 0x0103
--  record2.field1: 0x0203
--  record2: 0x02

-- View each record and field as their own attributes.
--  record1.field1: 0x01, 0x03
--  record2.field1: 0x02, 0x03
--  record2: 0x02

-- View each field as its own attribute.
-- View access to a record as access to every field in that record.
--  record1.field1: 0x03
--  record2.field1: 0x03
--  record2: 0x01, 0x03
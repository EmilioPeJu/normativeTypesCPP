/* ntnameValue.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTNAMEVALUE_H
#define NTNAMEVALUE_H

#include <pv/ntfield.h>

namespace epics { namespace nt {

class NTNameValue;
typedef std::tr1::shared_ptr<NTNameValue> NTNameValuePtr;

namespace detail {

    /**
     * Interface for in-line creating of NTNameValue.
     * One instance can be used to create multiple instances.
     * An instance of this object must not be used concurrently (an object has a state).
     * @author mse
     */
    class epicsShareClass NTNameValueBuilder :
        public std::tr1::enable_shared_from_this<NTNameValueBuilder>
    {
    public:
        POINTER_DEFINITIONS(NTNameValueBuilder);

        /**
         * Set a value array {@code Scalar} type.
         * @param scalarType value array scalar array.
         * @return this instance of a {@code NTTableBuilder}.
         */
        shared_pointer value(epics::pvData::ScalarType scalarType);

        /**
         * Add descriptor field to the NTNameValue.
         * @return this instance of a {@code NTNameValueBuilder}.
         */
        shared_pointer addDescriptor();

        /**
         * Add alarm structure to the NTNameValue.
         * @return this instance of a {@code NTNameValueBuilder}.
         */
        shared_pointer addAlarm();

        /**
         * Add timeStamp structure to the NTNameValue.
         * @return this instance of a {@code NTNameValueBuilder}.
         */
        shared_pointer addTimeStamp();

        /**
         * Create a {@code Structure} that represents NTNameValue.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of a {@code Structure}.
         */
        epics::pvData::StructureConstPtr createStructure();

        /**
         * Create a {@code PVStructure} that represents NTNameValue.
         * This resets this instance state and allows new {@code instance to be created.
         * @return a new instance of a {@code PVStructure}
         */
        epics::pvData::PVStructurePtr createPVStructure();

        /**
         * Create a {@code NTNameValue} instance.
         * This resets this instance state and allows new {@code instance to be created.
         * @return a new instance of a {@code NTNameValue}
         */
        NTNameValuePtr create();

        /**
         * Add extra {@code Field} to the type.
         * @param name name of the field.
         * @param field a field to add.
         * @return this instance of a {@code NTNameValueBuilder}.
         */
        shared_pointer add(std::string const & name, epics::pvData::FieldConstPtr const & field);

    private:
        NTNameValueBuilder();

        void reset();

        bool valueTypeSet;
        epics::pvData::ScalarType valueType;

        bool descriptor;
        bool alarm;
        bool timeStamp;

        // NOTE: this preserves order, however it does not handle duplicates
        epics::pvData::StringArray extraFieldNames;
        epics::pvData::FieldConstPtrArray extraFields;

        friend class ::epics::nt::NTNameValue;
    };

}

typedef std::tr1::shared_ptr<detail::NTNameValueBuilder> NTNameValueBuilderPtr;



/**
 * Convenience Class for NTNameValue
 * @author mrk
 */
class NTNameValue
{
public:
    POINTER_DEFINITIONS(NTNameValue);

    static const std::string URI;

    /**
     * Narrow (aka dynamic cast, or wrap) the structure to NTNameValue.
     * First the structure ID is checked against NTNameValue::URI.
     * This method will nullptr if the structure is nullptr.
     * @param structure The structure to narrow-ed (dynamic cast, wrapped) to NTNameValue.
     * @return NTNameValue instance on success, nullptr otherwise.
     */
    static shared_pointer narrow(epics::pvData::PVStructurePtr const & structure);

    /**
     * Narrow (aka dynamic cast, or wrap) the structure to NTNameValue without checking for null-ness or its ID.
     * @param structure The structure to narrow-ed (dynamic cast, wrapped) to NTNameValue.
     * @return NTNameValue instance.
     */
    static shared_pointer narrow_unsafe(epics::pvData::PVStructurePtr const & structure);

    /**
     * Is the structure an NTNameValue.
     * @param structure The structure to test.
     * @return (false,true) if (is not, is) an NTNameValue.
     */
    static bool is_a(epics::pvData::StructureConstPtr const & structure);

    /**
     * Create a NTNameValue builder instance.
     * @return builder instance.
     */
    static NTNameValueBuilderPtr createBuilder();

    /**
     * Destructor.
     */
    ~NTNameValue() {}

     /**
      * Attach a pvTimeStamp.
      * @param pvTimeStamp The pvTimeStamp that will be attached.
      * Does nothing if no timeStamp.
      * @return true if the operation was successfull (i.e. this instance has a timeStamp field), otherwise false.
      */
    bool attachTimeStamp(epics::pvData::PVTimeStamp &pvTimeStamp) const;

    /**
     * Attach an pvAlarm.
     * @param pvAlarm The pvAlarm that will be attached.
     * Does nothing if no alarm.
      * @return true if the operation was successfull (i.e. this instance has a timeStamp field), otherwise false.
     */
    bool attachAlarm(epics::pvData::PVAlarm &pvAlarm) const;

    /**
     * Get the pvStructure.
     * @return PVStructurePtr.
     */
    epics::pvData::PVStructurePtr getPVStructure() const;

    /**
     * Get the descriptor field.
     * @return The pvString or null if no function field.
     */
    epics::pvData::PVStringPtr getDescriptor() const;

    /**
     * Get the timeStamp.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getTimeStamp() const;

    /**
     * Get the alarm.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getAlarm() const;

    /**
     * Get the names array field.
     * @return The PVStringArray for the names.
     */
    epics::pvData::PVStringArrayPtr getNames() const;

    /**
     * Get the value array field.
     * @return The PVField for the values.
     */
    epics::pvData::PVFieldPtr getValues() const;

    /**
     * Get the value array field of a specified type (e.g. PVDoubleArray).
     * @return The <PVT> array for the values.
     */
    template<typename PVT>
    std::tr1::shared_ptr<PVT> getValues() const
    {
        epics::pvData::PVFieldPtr pvField = getValues();
        if (pvField.get())
            return std::tr1::dynamic_pointer_cast<PVT>(pvField);
        else
            return std::tr1::shared_ptr<PVT>();
    }

private:
    NTNameValue(epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvData::PVStructurePtr pvNTNameValue;
    friend class detail::NTNameValueBuilder;
};

}}
#endif  /* NTNAMEVALUE_H */

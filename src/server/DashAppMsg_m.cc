//
// Generated file, do not edit! Created by nedtool 5.4 from server/DashAppMsg.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include "DashAppMsg_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace {
template <class T> inline
typename std::enable_if<std::is_polymorphic<T>::value && std::is_base_of<omnetpp::cObject,T>::value, void *>::type
toVoidPtr(T* t)
{
    return (void *)(static_cast<const omnetpp::cObject *>(t));
}

template <class T> inline
typename std::enable_if<std::is_polymorphic<T>::value && !std::is_base_of<omnetpp::cObject,T>::value, void *>::type
toVoidPtr(T* t)
{
    return (void *)dynamic_cast<const void *>(t);
}

template <class T> inline
typename std::enable_if<!std::is_polymorphic<T>::value, void *>::type
toVoidPtr(T* t)
{
    return (void *)static_cast<const void *>(t);
}

}

namespace inet {

// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule to generate operator<< for shared_ptr<T>
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const std::shared_ptr<T>& t) { return out << t.get(); }

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');

    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(DashAppMsg)

DashAppMsg::DashAppMsg() : ::inet::FieldsChunk()
{
}

DashAppMsg::DashAppMsg(const DashAppMsg& other) : ::inet::FieldsChunk(other)
{
    copy(other);
}

DashAppMsg::~DashAppMsg()
{
}

DashAppMsg& DashAppMsg::operator=(const DashAppMsg& other)
{
    if (this == &other) return *this;
    ::inet::FieldsChunk::operator=(other);
    copy(other);
    return *this;
}

void DashAppMsg::copy(const DashAppMsg& other)
{
    this->expectedReplyLength = other.expectedReplyLength;
    this->contentLength = other.contentLength;
    this->replyDelay = other.replyDelay;
    this->serverClose = other.serverClose;
    this->StartByte = other.StartByte;
    this->EndByte = other.EndByte;
    this->redirectAddress = other.redirectAddress;
}

void DashAppMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::FieldsChunk::parsimPack(b);
    doParsimPacking(b,this->expectedReplyLength);
    doParsimPacking(b,this->contentLength);
    doParsimPacking(b,this->replyDelay);
    doParsimPacking(b,this->serverClose);
    doParsimPacking(b,this->StartByte);
    doParsimPacking(b,this->EndByte);
    doParsimPacking(b,this->redirectAddress);
}

void DashAppMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::FieldsChunk::parsimUnpack(b);
    doParsimUnpacking(b,this->expectedReplyLength);
    doParsimUnpacking(b,this->contentLength);
    doParsimUnpacking(b,this->replyDelay);
    doParsimUnpacking(b,this->serverClose);
    doParsimUnpacking(b,this->StartByte);
    doParsimUnpacking(b,this->EndByte);
    doParsimUnpacking(b,this->redirectAddress);
}

B DashAppMsg::getExpectedReplyLength() const
{
    return this->expectedReplyLength;
}

void DashAppMsg::setExpectedReplyLength(B expectedReplyLength)
{
    handleChange();
    this->expectedReplyLength = expectedReplyLength;
}

B DashAppMsg::getContentLength() const
{
    return this->contentLength;
}

void DashAppMsg::setContentLength(B contentLength)
{
    handleChange();
    this->contentLength = contentLength;
}

double DashAppMsg::getReplyDelay() const
{
    return this->replyDelay;
}

void DashAppMsg::setReplyDelay(double replyDelay)
{
    handleChange();
    this->replyDelay = replyDelay;
}

bool DashAppMsg::getServerClose() const
{
    return this->serverClose;
}

void DashAppMsg::setServerClose(bool serverClose)
{
    handleChange();
    this->serverClose = serverClose;
}

int DashAppMsg::getStartByte() const
{
    return this->StartByte;
}

void DashAppMsg::setStartByte(int StartByte)
{
    handleChange();
    this->StartByte = StartByte;
}

int DashAppMsg::getEndByte() const
{
    return this->EndByte;
}

void DashAppMsg::setEndByte(int EndByte)
{
    handleChange();
    this->EndByte = EndByte;
}

const char * DashAppMsg::getRedirectAddress() const
{
    return this->redirectAddress.c_str();
}

void DashAppMsg::setRedirectAddress(const char * redirectAddress)
{
    handleChange();
    this->redirectAddress = redirectAddress;
}

class DashAppMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
    enum FieldConstants {
        FIELD_expectedReplyLength,
        FIELD_contentLength,
        FIELD_replyDelay,
        FIELD_serverClose,
        FIELD_StartByte,
        FIELD_EndByte,
        FIELD_redirectAddress,
    };
  public:
    DashAppMsgDescriptor();
    virtual ~DashAppMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(DashAppMsgDescriptor)

DashAppMsgDescriptor::DashAppMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::DashAppMsg)), "inet::FieldsChunk")
{
    propertynames = nullptr;
}

DashAppMsgDescriptor::~DashAppMsgDescriptor()
{
    delete[] propertynames;
}

bool DashAppMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<DashAppMsg *>(obj)!=nullptr;
}

const char **DashAppMsgDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *DashAppMsgDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int DashAppMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 7+basedesc->getFieldCount() : 7;
}

unsigned int DashAppMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_expectedReplyLength
        FD_ISEDITABLE,    // FIELD_contentLength
        FD_ISEDITABLE,    // FIELD_replyDelay
        FD_ISEDITABLE,    // FIELD_serverClose
        FD_ISEDITABLE,    // FIELD_StartByte
        FD_ISEDITABLE,    // FIELD_EndByte
        FD_ISEDITABLE,    // FIELD_redirectAddress
    };
    return (field >= 0 && field < 7) ? fieldTypeFlags[field] : 0;
}

const char *DashAppMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "expectedReplyLength",
        "contentLength",
        "replyDelay",
        "serverClose",
        "StartByte",
        "EndByte",
        "redirectAddress",
    };
    return (field >= 0 && field < 7) ? fieldNames[field] : nullptr;
}

int DashAppMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0] == 'e' && strcmp(fieldName, "expectedReplyLength") == 0) return base+0;
    if (fieldName[0] == 'c' && strcmp(fieldName, "contentLength") == 0) return base+1;
    if (fieldName[0] == 'r' && strcmp(fieldName, "replyDelay") == 0) return base+2;
    if (fieldName[0] == 's' && strcmp(fieldName, "serverClose") == 0) return base+3;
    if (fieldName[0] == 'S' && strcmp(fieldName, "StartByte") == 0) return base+4;
    if (fieldName[0] == 'E' && strcmp(fieldName, "EndByte") == 0) return base+5;
    if (fieldName[0] == 'r' && strcmp(fieldName, "redirectAddress") == 0) return base+6;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *DashAppMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "inet::B",    // FIELD_expectedReplyLength
        "inet::B",    // FIELD_contentLength
        "double",    // FIELD_replyDelay
        "bool",    // FIELD_serverClose
        "int",    // FIELD_StartByte
        "int",    // FIELD_EndByte
        "string",    // FIELD_redirectAddress
    };
    return (field >= 0 && field < 7) ? fieldTypeStrings[field] : nullptr;
}

const char **DashAppMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *DashAppMsgDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int DashAppMsgDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    DashAppMsg *pp = (DashAppMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *DashAppMsgDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    DashAppMsg *pp = (DashAppMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string DashAppMsgDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    DashAppMsg *pp = (DashAppMsg *)object; (void)pp;
    switch (field) {
        case FIELD_expectedReplyLength: return unit2string(pp->getExpectedReplyLength());
        case FIELD_contentLength: return unit2string(pp->getContentLength());
        case FIELD_replyDelay: return double2string(pp->getReplyDelay());
        case FIELD_serverClose: return bool2string(pp->getServerClose());
        case FIELD_StartByte: return long2string(pp->getStartByte());
        case FIELD_EndByte: return long2string(pp->getEndByte());
        case FIELD_redirectAddress: return oppstring2string(pp->getRedirectAddress());
        default: return "";
    }
}

bool DashAppMsgDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    DashAppMsg *pp = (DashAppMsg *)object; (void)pp;
    switch (field) {
        case FIELD_expectedReplyLength: pp->setExpectedReplyLength(B(string2long(value))); return true;
        case FIELD_contentLength: pp->setContentLength(B(string2long(value))); return true;
        case FIELD_replyDelay: pp->setReplyDelay(string2double(value)); return true;
        case FIELD_serverClose: pp->setServerClose(string2bool(value)); return true;
        case FIELD_StartByte: pp->setStartByte(string2long(value)); return true;
        case FIELD_EndByte: pp->setEndByte(string2long(value)); return true;
        case FIELD_redirectAddress: pp->setRedirectAddress((value)); return true;
        default: return false;
    }
}

const char *DashAppMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *DashAppMsgDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    DashAppMsg *pp = (DashAppMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

} // namespace inet


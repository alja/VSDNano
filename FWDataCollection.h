#ifndef FWVCollection_h
#define FWVCollection_h

#include "ROOT/REveDataCollection.hxx"
#include "ROOT/REveDataProxyBuilderBase.hxx"
#include "nlohmann/json.hpp"

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

// A proxy-builder parameter, shown and edited in the GUI (VsdGed.controller.js).
// Kept as a plain struct, not as nlohmann::json: ROOT creates the TClass of every
// class-type data member when FWDataCollection's TClass is first used, and for
// nlohmann::json, which has no dictionary, that sends cling into a deep, slow lookup.
struct FWParameter
{
   enum EType { kBool, kLong, kDouble };

   std::string m_name;
   EType m_type{kLong};
   double m_val{0};
   bool m_hasMin{false};
   bool m_hasMax{false};
   double m_min{0};
   double m_max{0};
};

class FWDataCollection : public ROOT::Experimental::REveDataCollection
{
private:
    ROOT::Experimental::REveDataProxyBuilderBase *m_builder{nullptr};

    // config handling
    std::vector<FWParameter> m_parameters;

    static const char *typeName(FWParameter::EType t)
    {
        switch (t)
        {
        case FWParameter::kBool:   return "Bool";
        case FWParameter::kLong:   return "Long";
        case FWParameter::kDouble: return "Double";
        }
        return "Long";
    }

    FWParameter *findParameter(const std::string &name)
    {
        for (auto &p : m_parameters)
            if (p.m_name == name)
                return &p;
        return nullptr;
    }

    void addParameter(const std::string &name, FWParameter::EType type, double val)
    {
        if (findParameter(name))
            return;
        FWParameter p;
        p.m_name = name;
        p.m_type = type;
        p.m_val = val;
        m_parameters.push_back(p);
    }

public:
    // varconfig is the "var" array from the VSD file:
    // [ {"name": "MarkerSize", "type": "Long", "val": 4, "min": 1, "max": 10}, ... ]
    FWDataCollection(const std::string &n = "FWDataCollection", const std::string &varconfig = "") : ROOT::Experimental::REveDataCollection(n, "")
    {
        if (varconfig.empty())
            return;

        for (auto &elem : nlohmann::json::parse(varconfig))
        {
            FWParameter p;
            p.m_name = elem["name"];
            std::string type = elem["type"];
            if (type == "Bool")
                p.m_type = FWParameter::kBool;
            else if (type == "Long")
                p.m_type = FWParameter::kLong;
            else if (type == "Double")
                p.m_type = FWParameter::kDouble;
            else
            {
                printf("FWDataCollection %s: skipping parameter %s of unknown type %s\n", n.c_str(), p.m_name.c_str(), type.c_str());
                continue;
            }
            p.m_val = (p.m_type == FWParameter::kBool) ? double(elem["val"].get<bool>()) : elem["val"].get<double>();
            if (elem.contains("min"))
            {
                p.m_hasMin = true;
                p.m_min = elem["min"];
            }
            if (elem.contains("max"))
            {
                p.m_hasMax = true;
                p.m_max = elem["max"];
            }
            m_parameters.push_back(p);
        }
    }
    ~FWDataCollection() override {}

    bool hasConfigWithName(const std::string &n)
    {
        return findParameter(n) != nullptr;
    }

    // Add a parameter with a default value, unless the VSD file already set it.
    void assertParameter(const std::string &name, bool val) { addParameter(name, FWParameter::kBool, val); }
    void assertParameter(const std::string &name, long val) { addParameter(name, FWParameter::kLong, val); }

    void setGLBuilder(ROOT::Experimental::REveDataProxyBuilderBase *ib)
    {
        m_builder = ib;
    }

    int WriteCoreJson(nlohmann::json &j, int rnr_offset) override
    {
        int res = REveDataCollection::WriteCoreJson(j, -1);

        nlohmann::json var = nlohmann::json::array();
        for (auto &p : m_parameters)
        {
            nlohmann::json par = {{"name", p.m_name}, {"type", typeName(p.m_type)}};
            if (p.m_type == FWParameter::kBool)
                par["val"] = p.m_val != 0;
            else if (p.m_type == FWParameter::kLong)
                par["val"] = long(p.m_val);
            else
                par["val"] = p.m_val;
            if (p.m_hasMin)
                par["min"] = (p.m_type == FWParameter::kLong) ? nlohmann::json(long(p.m_min)) : nlohmann::json(p.m_min);
            if (p.m_hasMax)
                par["max"] = (p.m_type == FWParameter::kLong) ? nlohmann::json(long(p.m_max)) : nlohmann::json(p.m_max);
            var.push_back(par);
        }
        j["var"] = var;

        return res;
    }

    void UpdatePBParameter(char *name, char *val)
    {
        printf("Udate PB paramter %s %s \n", name, val);
        FWParameter *p = findParameter(name);
        if (p)
        {
            if (p->m_type == FWParameter::kBool)
            {
                p->m_val = (strcmp(val, "true") == 0);
            }
            else
            {
                char *eptr;
                errno = 0;
                double x = (p->m_type == FWParameter::kLong) ? double(strtol(val, &eptr, 10)) : strtod(val, &eptr);
                if (eptr == val || errno != 0)
                {
                    printf("Conversion error for parameter %s value %s\n", name, val);
                    return;
                }
                p->m_val = x;
            }
        }
        StampObjProps();
        if (m_builder)
            m_builder->Build();
    }

    long getLongParameter(const std::string &name)
    {
        FWParameter *p = findParameter(name);
        if (p && p->m_type == FWParameter::kLong)
            return long(p->m_val);
        printf("can't locate long paramter\n");
        return 0;
    }

    bool getBoolParameter(const std::string &name)
    {
        FWParameter *p = findParameter(name);
        if (p)
            return p->m_val != 0;
        printf("can't locate bool paramter\n");
        return false;
    }
};

#endif

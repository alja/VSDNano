#ifndef FWProvider_h
#define FWProvider_h

#include "Rtypes.h"
#include "TColor.h"

#include <string>
#include <vector>

namespace ROOT {
namespace Experimental {
class REveTableViewInfo;
}
}

// Description of one collection, as the event display needs it.
struct FWCollectionSpec
{
   std::string m_name;         // unique collection name
   std::string m_itemClass;    // TClass name of the items, e.g. "VsdJet" or "nanoaod::Jet"
   std::string m_proxyBuilder; // TClass name of the 3D proxy builder; empty for table only
   Color_t     m_color{kBlue};
   std::string m_filter;       // filter expression on the item, e.g. "i.pt() > 1"
   std::string m_varConfig;    // proxy-builder parameters as JSON, see FWDataCollection
};

struct FWEventId
{
   UInt_t   m_run{0};
   UInt_t   m_lumi{0};
   Long64_t m_event{0};
};

// Data source of the event display. The managers and the GUI only use this
// interface, so they work for any data format that can describe its
// collections and hand out item addresses.
class FWProvider
{
public:
   virtual ~FWProvider() {}

   // shown as the event display name, usually the file name
   virtual const std::string &GetTitle() const = 0;

   virtual Long64_t GetNumEvents() = 0;
   virtual Long64_t GetEventIdx() const = 0;
   virtual void     GotoEvent(int eventIdx) = 0;
   virtual FWEventId GetEventId() = 0;

   virtual std::vector<FWCollectionSpec> GetCollectionSpecs() = 0;

   // addresses of the items of the collection in the current event,
   // each of the class given in its FWCollectionSpec
   virtual void GetItems(const std::string &collection, std::vector<void *> &items) = 0;

   // columns of the table view, per item class
   virtual void SetupTables(ROOT::Experimental::REveTableViewInfo *) {}
};

#endif

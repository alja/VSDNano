// Event display service for VSD files, see FWService.h.
//
// A "load" request needs:
//   { "action": "load", "file": "<VSD file>", "user": "...", "logdir": "...", "logdirurl": "..." }

#include "VsdProvider.h"
#include "FWService.h"

class VsdService : public FWService
{
public:
   VsdService() : FWService("serviceVSDNano", 5555) {}

protected:
   void LoadLibraries() override
   {
      gSystem->Load("libVsdDict.so");
      gSystem->Load("libFWDict.so");
   }

   std::string DataFormat(TFile *file) override
   {
      if (!file)
         return "invalid";
      return file->Get("VSD") ? "vsd" : "edm";
   }

   FWProvider *CreateProvider(const nlohmann::json &req, std::string &error) override
   {
      std::string dataPath = req["file"].get<std::string>();
      printf("dataPath = %s \n", dataPath.c_str());

      std::unique_ptr<TFile> f(TFile::Open(dataPath.c_str()));
      if (!f || !f->Get("VSD"))
      {
         error = "Can not open file " + dataPath + " or it has no VSD tree.";
         return nullptr;
      }
      return new VsdProvider(dataPath.c_str());
   }
};

int main(int argc, char *argv[])
{
   VsdService service;
   return service.Main(argc, argv);
}

#include "TApplication.h"
#include "TKey.h"
#include "TEnv.h"

#include "VsdProvider.h"

#include "evd_main.cc"

////////////////////////////////////////////////////
int main(int argc, char **argv)
{
   if (argc != 2)
   {
      fprintf(stderr, "Usage: %s <VSD.root>\n", argv[0]);
      return 1;
   }

   const char *dummyArgvArray[] = {argv[0]};
   char **dummyArgv = const_cast<char **>(dummyArgvArray);

   int dummyArgc = 1;
   auto *app = new TApplication("evd-test", &dummyArgc, dummyArgv);

   auto *prov = new VsdProvider(argv[1]);

   evd_run(prov);

   app->Run();
   // REveManager::Create()/Show() owns the event loop.
   return 0;
}

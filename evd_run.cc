#include "TApplication.h"
#include "TKey.h"
#include "TEnv.h"

#include <cstdlib>
#include <cstring>

#include "VsdProvider.h"

#include "evd_main.cc"

////////////////////////////////////////////////////
int main(int argc, char **argv)
{
   int port = 10066; // default fixed http port; override with -port <int>
   const char *fname = nullptr;
   bool bad_args = false;
   for (int i = 1; i < argc; ++i)
   {
      if (!strcmp(argv[i], "-port") || !strcmp(argv[i], "--port"))
      {
         if (++i >= argc || (port = atoi(argv[i])) <= 0) bad_args = true;
      }
      else if (!fname)
         fname = argv[i];
      else
         bad_args = true;
   }
   if (bad_args || !fname)
   {
      fprintf(stderr, "Usage: %s <VSD.root> [-port <int>]\n", argv[0]);
      return 1;
   }

   const char *dummyArgvArray[] = {argv[0]};
   char **dummyArgv = const_cast<char **>(dummyArgvArray);

   int dummyArgc = 1;
   auto *app = new TApplication("evd-test", &dummyArgc, dummyArgv);

   // a fixed port instead of a random one from WebGui.HttpPortMin/Max;
   // if it is taken, ROOT falls back to a random port (see the log)
   gEnv->SetValue("WebGui.HttpPort", port);

   auto *prov = new VsdProvider(fname);

   evd_run(prov);

   app->Run();
   // REveManager::Create()/Show() owns the event loop.
   return 0;
}

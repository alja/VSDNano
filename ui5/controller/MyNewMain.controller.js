sap.ui.define(['rootui5/eve7/controller/Main.controller',
               'rootui5/eve7/lib/EveManager',
               "sap/ui/core/mvc/XMLView",
               'sap/ui/core/Fragment',
               'sap/m/MenuItem'
], function(MainController, EveManager, XMLView, Fragment, MenuItem) {
   "use strict";

   return MainController.extend("custom.MyNewMain", {

      onWebsocketClosed : function() {
         var elem = this.byId("centerTitle");
         elem.setHtmlText("<strong style=\"color: red;\">Client Disconnected !</strong>");
      },

      onInit: function() {
         console.log('MAIN CONTROLLER INIT 2');
         MainController.prototype.onInit.apply(this, arguments);
         this.mgr.handle.setReceiver(this);
         //this.mgr.
         console.log("register my controller for init");
         
         var elem = this.byId("centerTitle");
         let title = "<strong>" + elem.getProperty("htmlText")+ "</strong>";

let pthis = this;
         this.mgr.UT_refresh_invmass_dialog = function () {
            pthis.invMassDialogRefresh();
         }
          
         elem.setHtmlText(title);
      },

      onEveManagerInit: function() {

         MainController.prototype.onEveManagerInit.apply(this, arguments);
         var world = this.mgr.childs[0].childs;

         // this is a prediction that the fireworks GUI is the last element after scenes
         // could loop all the elements in top level and check for typename
         var last = world.length -1;

         if (world[last]._typename == "EventManager") {
            this.fw2gui = (world[last]);
            this.showEventInfo();
         }
      },

      updateViewers: function(loading_done) {
         let viewers = this.mgr.FindViewers();

         // first check number of views to create
         let staged = [];
         for (let n=0;n<viewers.length;++n) {
            let el = viewers[n];
            // at startup show only mandatory views
            if (typeof el.subscribed == 'undefined')
               el.subscribed = el.Mandatory;

            if (!el.$view_created && el.fRnrSelf) staged.push(el);
         }
         if (staged.length == 0) return;

         // swap 3d and rhoz view
         const temp = [staged[2], staged[0]];
         staged[0] = temp[0];
         staged[2] = temp[1];


         let vMenu = this.getView().byId("menuViewId");

         for (let n=0;n<staged.length;++n) {
            let eveView = staged[n];

            // add menu item
            let vi = new MenuItem({ text: staged[n].fName, press: this.subscribeView.bind(this, staged[n]) });
            vi.setEnabled(!eveView.subscribed);
            vi.eveView = eveView;
            vMenu.addItem(vi);

            eveView.$view_created = true;
            if(eveView.subscribed) this.makeEveViewController(eveView);
         }

         if (staged.length === 1) {
            let eveView = staged[0];
            let t = eveView.ca.byId("tbar");
            t.getContent()[2].setEnabled(false);
         }
      },

      showFWLog: function () {
            let idx = this.fw2gui.childs.length -1;
            sap.m.URLHelper.redirect(this.fw2gui.childs[idx].fTitle, true);
      },
      onWebsocketMsg : function(handle, msg, offset)
      {
         this.mgr.onWebsocketMsg(handle, msg, offset);
      },


      sceneElementChange: function(msg) {
         console.log("ddddddxxxxx sceneElementChange", msg)
      },

      showHelp : function(oEvent) {
         alert("=====User support: dummy@cern.ch");
      },

      eventFilterShow: function () {
         if (this.eventFilter){
            this.eventFilter.openFilterDialog();
         }
         else {
            let pthis = this;
            XMLView.create({
               viewName: "custom.view.EventFilter",
            }).then(function (oView) {
               pthis.eventFilter = oView.getController();
               pthis.eventFilter.setManager(pthis.mgr);
               pthis.eventFilter.setGUIElement(pthis.fw2gui);
              // console.log(oView, "filter dialog", oView.byId("filterDialog"));
               pthis.eventFilter.makeTables();
               pthis.eventFilter.openFilterDialog();
            }).catch(function (e) {
               console.error("Failed to open filter dialog:", e);
            });
         }
      },
      showEventInfo : function() {
         // console.log("showEventInfo");
         let ei = this.fw2gui.fTitle.split("/");
         var event = ei[0];
         var nevents = ei[1];
         var run = ei[2];
         var lumi = ei[3];
         let eiei = ei[4];
         document.title = this.fw2gui.fName +": " + event + "/" + nevents;

         this.byId("runInput").setValue(run);
         this.byId("lumiInput").setValue(lumi);
         this.byId("eventInput").setValue(eiei);

         this.byId("fileName").setText(this.fw2gui.fName);
         this.byId("fileName").setDesign("Bold");


         this.byId("fileNav").setText(event + "/" + nevents);
         this.byId("fileNav").setDesign("Bold");

         this.byId("projections").setValue(this.fw2gui.planeAngle);
      },

      nextEvent : function(oEvent) {
          this.mgr.SendMIR("NextEvent()", this.fw2gui.fElementId, "EventManager");
      },

      prevEvent : function(oEvent) {
         this.mgr.SendMIR("PreviousEvent()", this.fw2gui.fElementId, "EventManager");
      },

      toggleGedEditor: function() {
         this.byId("Summary").getController().toggleEditor();
      },

      onPressInvMass: function(oEvent)
      {
			var oButton = oEvent.getSource(),
			oView = this.getView();
            let pthis = this;
			// create popover
			if (!this._pPopover) {
				this._pPopover = Fragment.load({
					id: oView.getId(),
					name: "custom.view.InvMassPopover",
					controller: this
            }).then(function (oPopover) {
               oView.addDependent(oPopover);
					return oPopover;
				});
			}
			this._pPopover.then(function(oPopover) {
            pthis.fw2gui.childs[0].w = oPopover;

            let cl = oPopover.getContent();
            cl[0].setHtmlText("<pre>Press \'Calculate\' button to get result \nof current selection state</pre>");
				oPopover.openBy(oButton);
			});
      },
      handleInvMassCalcPress : function()
      {
		//	this.byId("myPopover").close();

         let inmd =  this.fw2gui.childs[0];
         this.mgr.SendMIR("Calculate()", inmd.fElementId, "InvMassDialog");
      },

      invMassDialogRefresh : function()
      {
         if (this.fw2gui) {
            let inmd = this.fw2gui.childs[0];
            if (inmd.w) {
               let cl = inmd.w.getContent();
               cl[0].setHtmlText(inmd.fTitle);
            }
         }
      },

      autoplay: function (oEvent) {
         console.log("AUTO", oEvent.getParameter("selected"));
         this.mgr.SendMIR("autoplay(" + oEvent.getParameter("selected") + ")", this.fw2gui.fElementId, "EventManager");
      },

      playdelay: function (oEvent) {
         console.log("playdelay ", oEvent.getParameter("value"));
         let pd_milisec = oEvent.getParameter("value") * 1000;
         this.mgr.SendMIR("playdelay(" + pd_milisec + ")", this.fw2gui.fElementId, "EventManager");
      },

      /*   onProjectionSubmit: function (oEvent) {
            var nValue = Number(oEvent.getParameter("value"));
   
            if (isNaN(nValue) || nValue < -180 || nValue > 180) {
               sap.m.MessageToast.show(
                  "Projection angle must be between -180 and 180."
               );
               return;
            }
   
            this.mgr.SendMIR(
               "setPlaneRotation(" + nValue + ", true)",
               this.fw2gui.fElementId,
               "EventManager"
            );
         }
         */onProjectionStepUp: function () {
   const oInput = this.byId("projections");
   let value = parseFloat(oInput.getValue());

   if (isNaN(value))
      value = 0;

   value += 1;

   if (value > 180)
      value = 180;

   value = Math.round(value * 100) / 100;

   oInput.setValue(value);
   this.onProjectionSubmit();
},

onProjectionStepDown: function () {
   const oInput = this.byId("projections");
   let value = parseFloat(oInput.getValue());

   if (isNaN(value))
      value = 0;

   value -= 1;

   if (value < -180)
      value = -180;

   value = Math.round(value * 100) / 100;

   oInput.setValue(value);
   this.onProjectionSubmit();
},

onProjectionSubmit: function (oEvent) {
   const oInput = this.byId("projections");
   let value = parseFloat(oInput.getValue());

   if (isNaN(value)) {
      oInput.setValue(0);
      return;
   }

   value = Math.round(value * 100) / 100;

   console.log("Projection:", value);

   this.mgr.SendMIR(
      "setPlaneRotation(" + value + ", true)",
      this.fw2gui.fElementId,
      "EventManager"
   );
}

   });
});

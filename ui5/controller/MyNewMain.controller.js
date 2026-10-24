sap.ui.define(['rootui5/eve7/controller/Main.controller',
               'rootui5/eve7/lib/EveManager',
               "sap/ui/core/mvc/XMLView",
               'sap/ui/core/Fragment',
               'sap/m/MenuItem',
               'rootui5/eve7/lib/GlViewerRCore',
               'rootui5/eve7/controller/EveTable.controller'
], function(MainController, EveManager, XMLView, Fragment, MenuItem, GlViewerRCore, EveTableController) {
   "use strict";

   // Cell text colours of the Table view in a dark theme; ROOT's eve.css sets
   // black and light gray, for a light background.
   const kDarkTableCss = `
.vsdTableCellPassDark { color: #e8e8e8; }
.vsdTableCellFailDark { color: #7d7d7d; }
`;

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

         // GL viewers take RQ_LineScale from the URL (default 1) and apply it in bootstrap();
         // without a URL value, use the input's current value instead (also for views opened later);
         // an explicit URL value wins and is shown in the input
         let url_scale = new URLSearchParams(window.location.search).get("RQ_LineScale");
         if (url_scale) {
            this.byId("lineScaleInput").setValue(url_scale);
         } else {
            let orig_bootstrap = GlViewerRCore.prototype.bootstrap;
            GlViewerRCore.prototype.bootstrap = function () {
               this.RQ_LineScale = parseFloat(pthis.byId("lineScaleInput").getValue()) || 1;
               return orig_bootstrap.apply(this, arguments);
            };
         }
      },

      onEveManagerInit: function() {

         MainController.prototype.onEveManagerInit.apply(this, arguments);
         var world = this.mgr.childs[0].childs;

         // this is a prediction that the fireworks GUI is the last element after scenes
         // could loop all the elements in top level and check for typename
         var last = world.length -1;

         if (world[last]._typename == "EventManager") {
            this.fw2gui = (world[last]);
            this.hookFileName();
            this.showEventInfo();
         }
      },

      // Refit the file name whenever it, the event counter next to it, or the
      // window changes size.
      hookFileName: function() {
         if (this.fileNameHooked) return;
         this.fileNameHooked = true;
         let refit = () => this.fitFileName();
         this.byId("fileName").addEventDelegate({ onAfterRendering: refit });
         this.byId("fileNav").addEventDelegate({ onAfterRendering: refit });
         window.addEventListener("resize", refit);
      },

      // Show the data file name in the room between GoToEvent and the event
      // counter. A path too long for it is trimmed at the start ("…/dir/file.root"),
      // so that it does not run under GoToEvent; the tooltip has the full name.
      fitFileName: function() {
         let full = this.fullFileName;
         let label = this.byId("fileName");
         if (!full || !label) return;
         label.setTooltip(full);

         let ld = label.getDomRef(), nd = this.byId("fileNav").getDomRef(), bar = this.byId("otb2").getDomRef();
         let mid = bar && bar.querySelector(".sapMBarPH");
         if (!ld || !nd || !mid) {
            if (label.getText() !== full) label.setText(full);
            return;
         }
         // Measure from where GoToEvent sits when centered: the Bar pushes its
         // middle content aside when the right side is too wide, and measuring
         // from its current place would let the name keep it there.
         let br = bar.getBoundingClientRect();
         let midRight = br.left + br.width / 2 + mid.getBoundingClientRect().width / 2;
         let navLeft = nd.getBoundingClientRect().left;
         let labelGap = Math.max(0, navLeft - ld.getBoundingClientRect().right); // margin before the counter
         let room = navLeft - labelGap - midRight - 24;                           // 24px clear of GoToEvent

         let ctx = this.measureCtx || (this.measureCtx = document.createElement("canvas").getContext("2d"));
         ctx.font = getComputedStyle(ld).font;
         let fits = t => ctx.measureText(t).width <= room;

         let text = full;
         if (!fits(text)) {
            let i = 1;
            while (i < full.length - 1 && !fits("\u2026" + full.substring(i))) ++i;
            text = "\u2026" + full.substring(i);
         }
         if (label.getText() !== text) label.setText(text);
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
            this.getSwapButton(eveView.ca).setEnabled(false);
         }
      },

      // the base class picks the swap button by fixed index (getContent()[2]), which breaks once GLTransformPatch adds its button in front of it; look it up by its tooltip instead
      getSwapButton: function (va) {
         return va.byId("tbar").getContent().find(c => c.getTooltip?.() === "swap");
      },

      setToolbarSwapIcon: function (va, iName) {
         this.getSwapButton(va)?.setIcon("sap-icon://" + iName);
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

         this.fullFileName = this.fw2gui.fName;
         this.byId("fileName").setDesign("Bold");
         this.fitFileName();


         this.byId("fileNav").setText(event + "/" + nevents);
         this.byId("fileNav").setDesign("Bold");

         this.byId("projections").setValue(this.fw2gui.planeAngle);

         this.byId("gotoEventInput").setValue(event);

         this.applyDarkMode(!!this.fw2gui.darkMode);
      },

      /** Dark mode is kept on the server (EventManager::setDarkMode), which also
        * gives the GL viewers a black background; here the GUI follows with a
        * dark UI5 theme, as FireworksWeb does. */
      onDarkModeChange: function(oEvent) {
         this.mgr.SendMIR("setDarkMode(" + oEvent.getParameter("state") + ")", this.fw2gui.fElementId, "EventManager");
      },

      applyDarkMode: function(dark) {
         this.byId("darkModeSwitch").setState(dark);
         if (this.themeIsDark === dark)
            return;

         if (this.lightThemeName === undefined) {
            // light and dark variant of the theme the page started with
            // (eventDisplay.html starts in sap_fiori_3_dark)
            let theme = sap.ui.getCore().getConfiguration().getTheme();
            this.lightThemeName = theme.replace(/_dark$/, "");
            this.darkThemeName = this.lightThemeName + "_dark";
            this.themeIsDark = theme.endsWith("_dark");
            this.setupDarkTableCells();
            if (this.themeIsDark === dark)
               return;
         }
         this.themeIsDark = dark;
         sap.ui.getCore().applyTheme(dark ? this.darkThemeName : this.lightThemeName);
      },

      /** The Table view writes its cells with fixed light-theme colours: use other
        * classes in the dark theme, and redraw the tables when the theme changes. */
      setupDarkTableCells: function() {
         let style = document.createElement("style");
         style.textContent = kDarkTableCss;
         document.head.appendChild(style);

         let pthis = this;
         let origGetCellText = EveTableController.prototype.getCellText;
         EveTableController.prototype.getCellText = function(value, filtered) {
            if (!pthis.themeIsDark)
               return origGetCellText.apply(this, arguments);
            return "<span class='" + (filtered ? "vsdTableCellPassDark" : "vsdTableCellFailDark") + "'>" + value + "</span>";
         };

         sap.ui.getCore().attachThemeChanged(this.refreshTables.bind(this));
         // tables built before getCellText was replaced, e.g. when the page starts dark
         this.refreshTables();
      },

      /** Rebuild the Table views, so that their cells use the current getCellText. */
      refreshTables: function() {
         this.mgr.FindViewers().forEach(function(viewer) {
            if (viewer.ca && viewer.ca._controllerName == "rootui5.eve7.controller.EveTable") {
               let ctrl = viewer.ca.getController();
               ctrl.refreshTable = true;
               ctrl.bindTableColumns = true;
               ctrl.endChanges();
            }
         });
      },

      nextEvent : function(oEvent) {
          this.mgr.SendMIR("NextEvent()", this.fw2gui.fElementId, "EventManager");
      },

      prevEvent : function(oEvent) {
         this.mgr.SendMIR("PreviousEvent()", this.fw2gui.fElementId, "EventManager");
      },

      gotoEvent : function(id) {
         let nevents = parseInt(this.fw2gui.fTitle.split("/")[1]);

         if (isNaN(id)) id = 0;
         id = Math.max(0, Math.min(id, nevents - 1));

         // show the clamped value
         this.byId("gotoEventInput").setValue(id);

         this.mgr.SendMIR("GotoEvent(" + id + ")", this.fw2gui.fElementId, "EventManager");
      },

      onGotoEventSubmit : function(oEvent) {
         this.gotoEvent(parseInt(oEvent.getParameter("value")));
      },

      toggleGedEditor: function() {
         this.byId("Summary").getController().toggleEditor();
      },

      onPressSettings: function(oEvent)
      {
         this.byId("settingsPopover").openBy(oEvent.getSource());
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

      setLineScale: function (scale) {
         for (let ctrl of this.mgr.gl_controllers) {
            let v = ctrl.viewer;
            if (!v?.creator || v.RQ_Mode == "Direct") continue;   // Direct mode never sets the factors
            v.RQ_LineScale = scale;
            v.creator.SetupPointLineFacs(v.RQ_SSAA,
                                         v.RQ_MarkerScale * v.canvas.pixelRatio,
                                         v.RQ_LineScale   * v.canvas.pixelRatio);
         }
         // rebuild the current event so its tracks pick up the new width
         this.mgr.SendMIR("GotoEvent(" + this.byId("gotoEventInput").getValue() + ")",
                          this.fw2gui.fElementId, "EventManager");
      },

      onLineScaleChange: function (oEvent) {
         let s = parseFloat(oEvent.getParameter("value"));
         if (s > 0) this.setLineScale(s);
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

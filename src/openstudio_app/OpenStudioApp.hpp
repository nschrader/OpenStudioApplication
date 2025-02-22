/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2020-2020, OpenStudio Coalition and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef OPENSTUDIO_OPENSTUDIOAPP_HPP
#define OPENSTUDIO_OPENSTUDIOAPP_HPP

#include "../openstudio_lib/SOConstants.hpp"
#include "../openstudio_lib/OSAppBase.hpp"

#include <openstudio/model/Model.hpp>
#include <openstudio/model/AirTerminalSingleDuctConstantVolumeNoReheat.hpp>
#include <openstudio/model/AirTerminalSingleDuctVAVReheat.hpp>
#include <openstudio/model/FanConstantVolume.hpp>
#include <openstudio/model/CurveQuadratic.hpp>
#include <openstudio/model/CurveBiquadratic.hpp>
#include <openstudio/model/AirLoopHVACOutdoorAirSystem.hpp>
#include <openstudio/model/ControllerOutdoorAir.hpp>
#include <openstudio/model/CoilHeatingGas.hpp>
#include <openstudio/model/CoilCoolingDXSingleSpeed.hpp>
#include <openstudio/model/EvaporativeCoolerDirectResearchSpecial.hpp>
#include <openstudio/model/ScheduleCompact.hpp>
#include <openstudio/model/SetpointManagerSingleZoneReheat.hpp>
#include <openstudio/model/SetpointManagerMixedAir.hpp>
#include <openstudio/model/SetpointManagerScheduled.hpp>
#include <openstudio/model/ChillerElectricEIR.hpp>
#include <openstudio/model/BoilerHotWater.hpp>
#include <openstudio/model/PumpVariableSpeed.hpp>
#include <openstudio/model/CoilCoolingWater.hpp>
#include <openstudio/model/CoilHeatingWater.hpp>
#include <openstudio/model/DistrictHeating.hpp>
#include <openstudio/model/DistrictCooling.hpp>

#include <openstudio/utilities/core/Path.hpp>

#include <QProcess>
#include <QFutureWatcher>

#include <vector>
#include <map>

class QEvent;

namespace openstudio {

namespace osversion {
class VersionTranslator;
}

class SystemComponent;

class MainWindow;

class AddToModelView;

class StartupView;

class OSDocument;

class StartupMenu;

class TouchEater : public QObject
{
  Q_OBJECT

 protected:
  bool eventFilter(QObject* obj, QEvent* event);
};

class OpenStudioApp : public OSAppBase
{

  Q_OBJECT

 public:
  OpenStudioApp(int& argc, char** argv);

  virtual ~OpenStudioApp();

  virtual std::shared_ptr<OSDocument> currentDocument() const override;

  static OpenStudioApp* instance();

  openstudio::model::Model componentLibrary() const;

  openstudio::model::Model hvacComponentLibrary() const;

  /** Returns an absolute, canonical path to the resource folder, whether it's launched from the build directory or the OS App **/
  openstudio::path resourcesPath() const;

  openstudio::path openstudioCLIPath() const;

  // Returns the hard set path (in settings), and or if not set will try to infer it by looking into the current PATH
  // If all fails, ends up returning an empty path (no need to wrap into a boost::optional (with overhead) for this)
  virtual openstudio::path dviewPath() const override;

  virtual bool notify(QObject* receiver, QEvent* event) override;

 protected:
  virtual bool event(QEvent* e) override;

  virtual void childEvent(QChildEvent* e) override;

 signals:

 public slots:

  void quit();

  void importIdf();

  void importgbXML();

  void importSDD();

  void importIFC();

  void open();

  void newModel();

  void showHelp();

  void checkForUpdate();

  void showAbout();

  virtual void reloadFile(const QString& osmPath, bool modified, bool saveCurrentTabs) override;

  void revertToSaved();

  // Loads the selected file (File > Load Library), and adds it to the user settings if not already there, by calling writeLibraryPaths
  void loadLibrary();

  // Loads the result of openstudio::model::exampleModel()
  void loadExampleModel();

  // Checks what happened in the LibraryDialog preference panes, and calls writeLibraryPaths to set the user settings
  void changeDefaultLibraries();

  // Checks what happened in the ExternalToolsDialog preference pane
  virtual void configureExternalTools() override;

 private slots:

  // Returns default/hvac_library.osm and default/hvac_library.osm
  std::vector<openstudio::path> defaultLibraryPaths() const;

  /**
   * Returns the current list of libraryPaths by reading the user settings
   * In the process, it convers any path marked "is_resource" to an absolute path
   * (This function is the reciprocal of writeLibraryPaths)
   */
  std::vector<openstudio::path> libraryPaths() const;

  // Build the component libraries and return a vector of paths that failed to load
  std::vector<std::string> buildCompLibraries();

  void newFromEmptyTemplateSlot();

  void newFromTemplateSlot(NewFromTemplateEnum newFromTemplateEnum);

  bool openFromDrag(QString path);

  bool closeDocument();

  void onCloseClicked();

  void measureManagerProcessStateChanged(QProcess::ProcessState newState);

  void measureManagerProcessFinished();

  void startMeasureManagerProcess();

  //void startMeasureManagerAndBuildCompLibraries();

  // This is the second half of the OSApp creation process.
  // Called after the comp libraries and measure manager are ready to go
  // Uses QApplication arguments to load osm files passed by cli into new OSDocument,
  // or creates a new empty OSDocument
  void onMeasureManagerAndLibraryReady();

  void onChangeDefaultLibrariesDone();

 private:
  enum fileType
  {
    SDD,
    GBXML
  };

  void showFailedLibraryDialog(const std::vector<std::string>& failedPaths);

  void import(fileType type);

  bool openFile(const QString& fileName, bool restoreTabs = false);

  void versionUpdateMessageBox(const osversion::VersionTranslator& translator, bool successful, const QString& fileName,
                               const openstudio::path& tempModelDir);

  void readSettings();

  void writeSettings();

  QString lastPath() const;

  void setLastPath(const QString& t_lastPath);

  void connectOSDocumentSignals();

  void disconnectOSDocumentSignals();

  // Removes the given path from the list of library settings (and calls writeLibraryPaths)
  void removeLibraryFromsSettings(const openstudio::path& path);

  /** Helper function that will write the library paths to the Settings indicating if it's a file within the resources/ folder or not
   * This will ensure that even if the user has selected 'resources/90_1_2013.osm' as a library, it'll keep on working with different versions
   * of OpenStudio (it wouldn't if we stored that as an absolute path)
   */
  void writeLibraryPaths(std::vector<openstudio::path> paths);

  int startTabIndex() const;

  QProcess* m_measureManagerProcess;

  openstudio::model::Model m_compLibrary;

  openstudio::model::Model m_hvacCompLibrary;

  openstudio::model::Model m_library;

  std::shared_ptr<OSDocument> m_osDocument;

  QString m_lastPath;

  // Try to find DView (or DView.exe) inside the PATH env variable. Will return an *empty* path if couldn't infer it
  openstudio::path inferredDViewPath() const;
  openstudio::path m_dviewPath;
  void setDviewPath(const openstudio::path& t_dviewPath);

  std::shared_ptr<StartupMenu> m_startupMenu;

  QFutureWatcher<std::vector<std::string>> m_buildCompLibWatcher;
  QFutureWatcher<bool> m_waitForMeasureManagerWatcher;
  QFutureWatcher<std::vector<std::string>> m_changeLibrariesWatcher;
};

}  // namespace openstudio

#endif  // OPENSTUDIO_OPENSTUDIOAPP_HPP

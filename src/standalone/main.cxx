#include <QtGui>
#include "BL_GenericGui.hxx"
#include "BL_MainWindows_Qt.hxx"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  QMainWindow main_window;
  BL::MainWindows_Qt wrapper(&main_window);
  BL::GenericGui * main_widget = new BL::GenericGui(&wrapper);
  main_widget->createActions();
  main_widget->createMenus();
  main_widget->updateButtonsStates();

  main_window.setWindowTitle("Standalone JobManager Gui");
  main_window.show();

  return app.exec();
}


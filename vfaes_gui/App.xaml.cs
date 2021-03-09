using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace vfaes_gui
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {

        // Primary instance
        public static App Instance { get; set; }

        // Primary window
        public static MainWindow Window { get; set; }

        App()
        {
            Instance = this;
        }

        // Router for telling main window to pop first element from request list
        public static void UpdateEncryptionComplete(bool success)
        {
            Instance.Dispatcher.Invoke(
                    () => 
                    {
                        Window.PopView(success);
                    }
                );
        }
    }
}

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace vfaes_gui
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        public MainWindow()
        {
            InitializeComponent();

            App.Window = this;

            // Start event queue
            EncryptorManager.Start();
        }

        // Get key
        private string GetKey()
        {
            string key = KeyTextBox.Text;

            if (key.Length == 0)
            {
                key = "default";
            }

            return key;
        }

        // Adds label to RequestQueueView
        private void AddLabelToRequestQueueView(EncryptionRequest request)
        {
            var label = new Label
            {
                Content = request.FileLocation
            };

            RequestQueueView.Children.Add(label);
        }

        private void DragStackPanel_Drop(object sender, DragEventArgs e)
        {

            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                // Get all files
                string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);

                // Push to queue
                foreach (var file in files)
                {
                    var request = new EncryptionRequest(file, GetKey());
                    // Add to queue
                    EncryptorManager.RequestEncrypt(request);

                    // Add a label
                    AddLabelToRequestQueueView(request);
                }
            }
        }

        private void MainWindow_Closing(object sender, CancelEventArgs e)
        {
            EncryptorManager.Stop();
        }

        // Index used to keep track of current start
        private int CurrentRequestIndex = 0;

        // Pop the first element from the queue view
        public void PopView(bool success)
        {
            // If successful remove label
            if (success)
            {
                if (RequestQueueView.Children.Count > CurrentRequestIndex)
                {
                    RequestQueueView.Children.RemoveAt(CurrentRequestIndex);
                }
            }
            // Otherwise increase index
            else
            {
                // Set current label as bad and increment
                var label = (Label)RequestQueueView.Children[CurrentRequestIndex];
                label.Foreground = Brushes.Red;
                label.Content = "Invalid Key " + label.Content;

                CurrentRequestIndex++;
            }
            
        }
    }
}

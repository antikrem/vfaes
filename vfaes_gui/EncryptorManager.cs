using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace vfaes_gui
{
    // Represents a single request to 
    // encrypt, including
    // A file and key
    public class EncryptionRequest
    {
        // Absolute location of file
        public string FileLocation
        {
            get;
            set;
        }

        // Key used in encryption 
        // In readable form (hashing done by vfaes.exe)
        public string Key
        {
            get;
            set;
        }

        public EncryptionRequest(string fileLocation, string key)
        {
            FileLocation = fileLocation;
            Key = key;
        }
    }

    // Manages the Encryption process
    static class EncryptorManager
    {
        // File name of encrypting executable
        private const string EXECUTABLE_FILE = "vfaes.exe";

        // Int flag for success
        private const int SUCCESS = 0;

        // Queue of encryption requests
        private static readonly BlockingCollection<EncryptionRequest> RequestQueue
                = new BlockingCollection<EncryptionRequest>(new ConcurrentQueue<EncryptionRequest>());

        // Reference to worker thread
        private static Thread WorkerThread = null;

        // Take a request and manage its execution
        // Returns exit code indicating success of operation
        private static int ExecuteSingleEncryption(EncryptionRequest request)
        {
            // Expected arguments
            string[] arguments = { "-key", request.Key, request.FileLocation };

            // Standard process properties
            ProcessStartInfo setup = new ProcessStartInfo
            {
                FileName = EXECUTABLE_FILE,
                Arguments = string.Join(" ", arguments),
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                UseShellExecute = false,
                CreateNoWindow = true
            };

            // Start VFAES
            var process = System.Diagnostics.Process.Start(setup);

            // Wait for exit and return success
            process.WaitForExit();

            return process.ExitCode;
        }

        // Main body event handling loop
        private static void EventLoop()
        {

            EncryptionRequest request;

            // Constant while loop while requied
            while ((request = RequestQueue.Take()) != null)
            {
                var success = ExecuteSingleEncryption(request) ;
                App.UpdateEncryptionComplete((success == SUCCESS));
            }

            // This is application end
        }

        // Start background worker thread
        public static void Start()
        {
            WorkerThread = new Thread(EventLoop);
            WorkerThread.Start();
        }

        // Stop application and end
        public static void Stop()
        {
            // Null ends event queue
            RequestQueue.Add(null);


            // Just wait for the thread to end
            WorkerThread.Join();
        }

        // Add an encryption request
        public static void RequestEncrypt(EncryptionRequest request)
        {
            RequestQueue.Add(request);
        }
    }
}

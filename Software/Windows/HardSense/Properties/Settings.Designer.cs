﻿//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.42000
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

namespace HardSense.Properties {
    
    
    [global::System.Runtime.CompilerServices.CompilerGeneratedAttribute()]
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.VisualStudio.Editors.SettingsDesigner.SettingsSingleFileGenerator", "16.4.0.0")]
    internal sealed partial class Settings : global::System.Configuration.ApplicationSettingsBase {
        
        private static Settings defaultInstance = ((Settings)(global::System.Configuration.ApplicationSettingsBase.Synchronized(new Settings())));
        
        public static Settings Default {
            get {
                return defaultInstance;
            }
        }
        
        [global::System.Configuration.ApplicationScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("HardSense-MMFILE-V1")]
        public string MemoryMapFileName {
            get {
                return ((string)(this["MemoryMapFileName"]));
            }
        }
        
        [global::System.Configuration.ApplicationScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("10")]
        public int DefaultMemoryMappedFieldSize {
            get {
                return ((int)(this["DefaultMemoryMappedFieldSize"]));
            }
        }
        
        [global::System.Configuration.ApplicationScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("/recv")]
        public string DefaultNicReceiveID {
            get {
                return ((string)(this["DefaultNicReceiveID"]));
            }
        }
        
        [global::System.Configuration.ApplicationScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("/send")]
        public string DefaultNicSendID {
            get {
                return ((string)(this["DefaultNicSendID"]));
            }
        }
        
        [global::System.Configuration.ApplicationScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("Receive Speed")]
        public string DefaultNicReceiveSensorName {
            get {
                return ((string)(this["DefaultNicReceiveSensorName"]));
            }
        }
        
        [global::System.Configuration.ApplicationScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("Send Speed")]
        public string DefaultNicSendSensorName {
            get {
                return ((string)(this["DefaultNicSendSensorName"]));
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("250")]
        public int DefaultHardwareMonitorPollTime {
            get {
                return ((int)(this["DefaultHardwareMonitorPollTime"]));
            }
            set {
                this["DefaultHardwareMonitorPollTime"] = value;
            }
        }
        
        [global::System.Configuration.ApplicationScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("NIC")]
        public string DefaultNetworkInterfaceType {
            get {
                return ((string)(this["DefaultNetworkInterfaceType"]));
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("4121")]
        public int DefaultDataStreamingServerPort {
            get {
                return ((int)(this["DefaultDataStreamingServerPort"]));
            }
            set {
                this["DefaultDataStreamingServerPort"] = value;
            }
        }
        
        [global::System.Configuration.ApplicationScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("128")]
        public int DefaultMaxLengthDataToSend {
            get {
                return ((int)(this["DefaultMaxLengthDataToSend"]));
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("2000")]
        public int DefaultHeartbeatTime {
            get {
                return ((int)(this["DefaultHeartbeatTime"]));
            }
            set {
                this["DefaultHeartbeatTime"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("3")]
        public int DefaultMissedHeartbeatsToDisconnect {
            get {
                return ((int)(this["DefaultMissedHeartbeatsToDisconnect"]));
            }
            set {
                this["DefaultMissedHeartbeatsToDisconnect"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("")]
        public string DefaultBTSerialPort {
            get {
                return ((string)(this["DefaultBTSerialPort"]));
            }
            set {
                this["DefaultBTSerialPort"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("0, 0")]
        public global::System.Drawing.Point BluetoothConfiguratorFormLastPosition {
            get {
                return ((global::System.Drawing.Point)(this["BluetoothConfiguratorFormLastPosition"]));
            }
            set {
                this["BluetoothConfiguratorFormLastPosition"] = value;
            }
        }
        
        [global::System.Configuration.ApplicationScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("3")]
        public int DefaultBTConnectMaxFailures {
            get {
                return ((int)(this["DefaultBTConnectMaxFailures"]));
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("False")]
        public bool DebugOutput {
            get {
                return ((bool)(this["DebugOutput"]));
            }
            set {
                this["DebugOutput"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("500")]
        public int DefaultSensorDataStreamer_UpdateTime {
            get {
                return ((int)(this["DefaultSensorDataStreamer_UpdateTime"]));
            }
            set {
                this["DefaultSensorDataStreamer_UpdateTime"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("0, 0")]
        public global::System.Drawing.Point MainFormLastPosition {
            get {
                return ((global::System.Drawing.Point)(this["MainFormLastPosition"]));
            }
            set {
                this["MainFormLastPosition"] = value;
            }
        }
        
        [global::System.Configuration.ApplicationScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("/fps/0/counter")]
        public string FPSSensorName {
            get {
                return ((string)(this["FPSSensorName"]));
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("0, 0")]
        public global::System.Drawing.Point LayoutEditor_HomeScreenLastPosition {
            get {
                return ((global::System.Drawing.Point)(this["LayoutEditor_HomeScreenLastPosition"]));
            }
            set {
                this["LayoutEditor_HomeScreenLastPosition"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("0, 0")]
        public global::System.Drawing.Point SensorMonitor_WindowsLastPosition {
            get {
                return ((global::System.Drawing.Point)(this["SensorMonitor_WindowsLastPosition"]));
            }
            set {
                this["SensorMonitor_WindowsLastPosition"] = value;
            }
        }
    }
}

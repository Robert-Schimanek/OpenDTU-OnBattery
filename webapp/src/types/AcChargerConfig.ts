export interface AcChargerConfig {
    enabled: boolean;
    verbose_logging: boolean;
    can_controller_frequency: number;
    max_current_multiplier: number;
    auto_power_enabled: boolean;
    auto_power_batterysoc_limits_enabled: boolean;
    voltage_limit: number;
    enable_voltage_limit: number;
    lower_power_limit: number;
    upper_power_limit: number;
    emergency_charge_enabled: boolean;
    stop_batterysoc_threshold: number;
    target_power_consumption: number;
    target_huawei_data_request_interval: number;
    charger_meter_not_charger: boolean;
    surplus_charge_factor: number;
}

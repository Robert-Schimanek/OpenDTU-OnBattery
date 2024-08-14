import type { HttpRequestConfig } from '@/types/HttpRequestConfig';

export interface PowerMeterChargerMqttValue {
    topic: string;
    json_path: string;
    unit: number;
    sign_inverted: boolean;
}

export interface PowerMeterChargerMqttConfig {
    values: Array<PowerMeterChargerMqttValue>;
}

export interface PowerMeterChargerSerialSdmConfig {
    polling_interval: number;
    address: number;
}

export interface PowerMeterChargerHttpJsonValue {
    http_request: HttpRequestConfig;
    enabled: boolean;
    json_path: string;
    unit: number;
    sign_inverted: boolean;
}

export interface PowerMeterChargerHttpJsonConfig {
    polling_interval: number;
    individual_requests: boolean;
    values: Array<PowerMeterChargerHttpJsonValue>;
}

export interface PowerMeterChargerHttpSmlConfig {
    polling_interval: number;
    http_request: HttpRequestConfig;
}

export interface PowerMeterChargerConfig {
    enabled: boolean;
    verbose_logging: boolean;
    source: number;
    interval: number;
    mqtt: PowerMeterChargerMqttConfig;
    serial_sdm: PowerMeterChargerSerialSdmConfig;
    http_json: PowerMeterChargerHttpJsonConfig;
    http_sml: PowerMeterChargerHttpSmlConfig;
}

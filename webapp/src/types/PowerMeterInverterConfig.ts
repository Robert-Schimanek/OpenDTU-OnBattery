import type { HttpRequestConfig } from '@/types/HttpRequestConfig';

export interface PowerMeterInverterMqttValue {
    topic: string;
    json_path: string;
    unit: number;
    sign_inverted: boolean;
}

export interface PowerMeterInverterMqttConfig {
    values: Array<PowerMeterInverterMqttValue>;
}

export interface PowerMeterInverterSerialSdmConfig {
    polling_interval: number;
    address: number;
}

export interface PowerMeterInverterHttpJsonValue {
    http_request: HttpRequestConfig;
    enabled: boolean;
    json_path: string;
    unit: number;
    sign_inverted: boolean;
}

export interface PowerMeterInverterHttpJsonConfig {
    polling_interval: number;
    individual_requests: boolean;
    values: Array<PowerMeterInverterHttpJsonValue>;
}

export interface PowerMeterInverterHttpSmlConfig {
    polling_interval: number;
    http_request: HttpRequestConfig;
}

export interface PowerMeterInverterConfig {
    enabled: boolean;
    verbose_logging: boolean;
    source: number;
    interval: number;
    mqtt: PowerMeterInverterMqttConfig;
    serial_sdm: PowerMeterInverterSerialSdmConfig;
    http_json: PowerMeterInverterHttpJsonConfig;
    http_sml: PowerMeterInverterHttpSmlConfig;
}

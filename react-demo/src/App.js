import React, { useState, useEffect, useRef } from "react";
import "./App.css";

const App = () => {
    const [userName, setUserName] = useState("");
    const [messages, setMessages] = useState([]);
    const [inputMessage, setInputMessage] = useState("");
    const websocketRef = useRef();

    useEffect(() => {
        websocketRef.current = new WebSocket("ws://localhost:30080");
        websocketRef.current.onmessage = (event) => {
            const message = JSON.parse(event.data);
            setMessages((prevMessages) => [...prevMessages, message]);
        };

        // Clean up
        return () => {
            websocketRef.current.close();
        };
    }, []);

    const sendMessage = () => {
        if (userName === "") {
            alert("Please enter your name before sending a message.");
            return;
        }
        displayMessage();
        const message = {
            type: "message",
            content: inputMessage,
            sender: "user",
            userName: userName,
        };
        websocketRef.current.send(JSON.stringify(message));
        setInputMessage("");
    };

    const displayMessage = () => {
        const message = {
            type: "message",
            content: inputMessage,
            sender: "user",
            userName: userName,
        };
        setMessages((prevMessages) => [...prevMessages, message]);
    };

    return (
        <div className="App">
            <div className="name-input">
                <input
                    type="text"
                    placeholder="Enter your name"
                    value={userName}
                    onChange={(e) => setUserName(e.target.value)}
                />
            </div>
            <div className="messages-container">
                {messages.map((message, index) => (
                    <div
                        key={index}
                        className={`message ${
                            message.userName === userName
                                ? "user-message"
                                : "partner-message"
                        }`}
                    >
                        <strong>{message.userName}: </strong>
                        {message.content}
                    </div>
                ))}
            </div>

            <div className="input-container">
                <input
                    type="text"
                    value={inputMessage}
                    onChange={(e) => setInputMessage(e.target.value)}
                />
                <button onClick={sendMessage}>Send</button>
            </div>
        </div>
    );
};

export default App;
